# [📬 Automail UI](https://automail-ui.anothersimplecoder.de/)

> Bulk-delete emails from specific senders — without opening your inbox.

Automail UI is a lightweight desktop app with a **C++20 native backend** and an **Angular frontend** rendered inside an embedded WebView. No browser, no Electron, no runtime dependencies.

| | |
|---|---|
| 📡 **IMAP** | Any IMAP server — Gmail, Outlook.com, self-hosted, you name it |
| 🪟 **COM / Outlook** | Windows only — hooks directly into a locally installed Outlook session |

---

## 📖 Table of Contents

- [⚙️ How it works](#️-how-it-works)
- [✅ Prerequisites](#-prerequisites)
- [🔨 Building](#-building)
- [🗂️ Configuration](#️-configuration)
- [🚀 Usage](#-usage)
- [🗃️ Project structure](#️-project-structure)
- [🤝 Contributing](#-contributing)

---

## ⚙️ How it works

```
┌─────────────────────────────────────────────┐
│                  main.exe                   │
│                                             │
│  ┌──────────────┐    JS bridge    ┌───────┐ │
│  │   WebView    │◄───────────────►│  C++  │ │
│  │  (Angular)   │                 │  core │ │
│  └──────┬───────┘                 └───┬───┘ │
│         │ HTTP GET                    │     │
│  ┌──────▼───────┐             ┌───────▼───┐ │
│  │ cpp-httplib  │             │   Mail    │ │
│  │ (embedded)   │             │  service  │ │
│  └──────────────┘             └───────────┘ │
└─────────────────────────────────────────────┘
```

1. 🟢 On startup, the C++ backend binds an HTTP server to a random localhost port and serves the Angular SPA **from memory** — assets are compiled directly into the binary, so no `dist/` folder is needed at runtime.
2. 🌐 The native WebView window navigates to `http://localhost:<port>`.
3. 🔗 The Angular app talks to C++ through three **webview JS bindings**: `doDelete`, `saveImapConfig`, and `loadImapConfig`.
4. 🗑️ The mail service connects to the configured server and permanently deletes matching messages.

---

## ✅ Prerequisites

### 🌍 All platforms

| Tool | Version | Install |
|------|---------|---------|
| CMake | ≥ 4.3 | [cmake.org](https://cmake.org/download/) |
| Conan | ≥ 2.0 | `pip install conan` |
| Node.js | ≥ 22 | [nodejs.org](https://nodejs.org/) |

### 🪟 Windows (additional)

| Tool | Notes |
|------|-------|
| Visual Studio 2022 | Community edition is fine — needs the **Desktop development with C++** workload |
| Outlook | Only required if using the COM client |

### 🐧 Linux (additional)

```bash
sudo apt-get install -y \
    build-essential ninja-build python3 python3-pip \
    libssl-dev libgtk-3-dev libwebkit2gtk-4.1-dev
```

---

## 🔨 Building

### 🪟 Windows

```powershell
# 1️⃣ Build the Angular frontend
cd frontend
npm ci
npm run build
cd ..

# 2️⃣ Configure — Conan fetches C++ deps, CMake embeds the dist bundle
conan profile detect --force   # first time only
cmake -S . -B build `
      -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake `
      -DCMAKE_BUILD_TYPE=Release `
      -G "Visual Studio 17 2022"

# 3️⃣ Compile
cmake --build build --config Release
```

📦 Output: `build\src\main\Release\main.exe`

> 💡 **After every `ng build`**, CMake automatically detects the changed dist files and re-embeds them on the next `cmake --build` — no manual reconfigure needed.

---

### 🐧 Linux

```bash
# 1️⃣ Build the Angular frontend
cd frontend && npm ci && npm run build && cd ..

# 2️⃣ Configure & compile
pip install conan
conan profile detect --force   # first time only
cmake -S . -B build \
      -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake \
      -DCMAKE_BUILD_TYPE=Release \
      -G Ninja
cmake --build build
```

📦 Output: `build/src/main/main`

---

### 🐳 Docker

Produces a minimal Ubuntu 24.04 image with the self-contained binary inside. The Angular bundle is baked in — no `dist/` folder needed in the image.

```bash
# Build the image
docker build -t automail_ui .

# Run — mount your config from the host
docker run -v $(pwd)/config.yml:/app/config.yml automail_ui
```

---

## 🗂️ Configuration

On first run the app reads `config.yml` from its working directory:

```yaml
# Which backend to use: "imap" or "com"
client_type: imap

imap:
  hostname: imap.example.com
  port: 993           # 993 = SSL, 143 = plain
  username: you@example.com
  password: your_password
  ssl: true
  mailbox: INBOX
```

> ⚠️ **Security:** The password is stored in plaintext. Do **not** commit `config.yml` — add it to `.gitignore`.

All settings can also be changed at runtime through the **Settings** view inside the app; changes are written back to `config.yml` immediately.

---

## 🚀 Usage

1. **Launch** the app — a native window opens with the Angular UI.
2. Go to ⚙️ **Settings**, configure your mail connection, and hit **Save**.
3. Switch to 📋 **Senders** and add the email addresses you want to purge.
4. Click **Process & Delete** — the backend finds and permanently removes every matching message.

> 💡 The **COM client** requires a running, locally installed Outlook. No credentials needed — it reuses Outlook's existing authenticated session.

---

## 🗃️ Project structure

```
automail_ui/
│
├── 📁 cmake/
│   └── embed_resources.cmake     # Generates RC / C++ byte-array files from dist/
│
├── 📁 frontend/                  # Angular application
│   ├── src/app/
│   │   ├── views/
│   │   │   ├── senders/          # Sender list + delete trigger
│   │   │   └── settings/         # IMAP / COM configuration form
│   │   ├── header/               # App bar + theme toggle
│   │   ├── sidebar/              # Navigation
│   │   ├── services/
│   │   │   └── theme.service.ts  # Dark / light mode
│   │   ├── app.model.ts          # Shared TypeScript interfaces
│   │   └── native-bridge.ts      # window.doDelete / saveImapConfig / loadImapConfig
│   └── dist/                     # Build output — embedded into the binary at compile time
│
├── 📁 include/                   # C++ headers
│   ├── config/                   # Config struct, factory, store
│   ├── mail/
│   │   ├── mail_service.hh
│   │   └── clients/
│   │       ├── mail_client.hh    # Abstract client interface
│   │       ├── imap_client.hh
│   │       ├── client_factory.hh
│   │       └── com/              # Windows COM / Outlook client
│   ├── gui/                      # WebView window + JS function handler
│   ├── filesystem_service.hh
│   └── frontend_server.hh        # Embedded HTTP server
│
├── 📁 src/                       # C++ implementations (mirrors include/)
│
├── 🐳 Dockerfile
├── ⚙️  .gitlab-ci.yml
├── 📄 CMakeLists.txt
└── 🔑 config.yml                 # Runtime config — do not commit
```

---

## 🤝 Contributing

### 🏗️ Architecture

The backend is split into four clear layers — keep them separate:

| Layer | Location | Responsibility |
|-------|----------|---------------|
| 🔌 **Entry point** | `src/main/main.cpp` | Wires all components together |
| 📬 **Mail** | `src/mail/` + `include/mail/` | Protocol clients and deletion logic |
| 🗄️ **Config** | `src/config/` + `include/config/` | Loading, validating, and persisting settings |
| 🖥️ **GUI** | `src/gui/` + `include/gui/` | WebView window, JS bindings, HTTP server |

> 📐 **Golden rule:** Business logic lives in C++. Presentation lives in Angular. The only bridge between them is the three functions on `window`.

---

### 📬 Adding a new mail client

<details>
<summary>Click to expand</summary>

**1. Create a header** in `include/mail/clients/` inheriting from `client`:

```cpp
// include/mail/clients/my_client.hh
#pragma once
#include "mail_client.hh"

class my_client : public client {
public:
    void init() override;
    void delete_messages_from(const std::vector<std::string>& senders) override;
};
```

**2. Implement it** in `src/mail/clients/my_client/my_client.cc`.

**3. Add a `CMakeLists.txt`** in the same directory and register the target in `src/mail/clients/CMakeLists.txt`.

**4. Register a new `client_type`** in `include/config/config.hh` and add the factory branch in `client_factory.cc`.

**5. Expose it in the UI** — add the value to `ClientType` in `app.model.ts` and add a settings section in `views/settings/settings.html`.

</details>

---

### 🎨 Modifying the frontend

<details>
<summary>Click to expand</summary>

Run Angular in dev mode with hot reload (no C++ build needed):

```bash
cd frontend
npm ci
npm run start    # → http://localhost:4200
```

`native-bridge.ts` exports `isNative()` — it returns `false` in the browser, so you can branch on it to provide mock data during development.

When you're done:

```bash
npm run build         # → frontend/dist/frontend/browser/
cmake --build build   # CMake auto-detects changed dist files and recompiles
```

**Adding a new view:**

1. Create `frontend/src/app/views/my-view/` with `my-view.ts` + `my-view.html`.
2. Declare it as a standalone component.
3. Add a nav entry in `sidebar/sidebar.ts` and wire the toggle in `app.ts`.

</details>

---

### 🔗 Adding a new JS binding

<details>
<summary>Click to expand</summary>

**1. Declare the signature** in `native-bridge.ts`:
```typescript
interface Window {
    myNewFunction: (arg: MyType) => Promise<string>;
}
```

**2. Register the binding** in `src/gui/function_handler.cc`:
```cpp
window_.bind("myNewFunction", [&](std::string args) -> std::string {
    auto parsed = json::parse(args);
    // ... call your C++ service ...
    return "{}";
});
```

**3. Call it** from Angular:
```typescript
const result = await window.myNewFunction(payload);
```

</details>

---

### 🖊️ Code style

**C++**
- Standard: **C++20**
- Naming: `snake_case` for everything
- Headers → `.hh` · Sources → `.cc`
- One class per directory under `src/` and `include/`
- Logging: `spdlog::info` / `spdlog::error` — never `std::cout`

**Angular / TypeScript**
- Standalone components only — no `NgModule`
- Angular Signals for reactive state
- Filenames in `kebab-case`
- Keep components thin — state and side effects belong in services
