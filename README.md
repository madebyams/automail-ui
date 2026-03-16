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
3. 🔗 The Angular app talks to C++ through four **webview JS bindings**: `hasConfig`, `doDelete`, `saveImapConfig`, and `loadImapConfig`.
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

# Run — mount a persistent directory for the config database
docker run -v $(pwd)/data:/app automail_ui
```

---

## 🗂️ Configuration

Configuration is stored in a **SQLite database** (`config.db`) in the app's working directory — no config file needed.

On first launch, the app detects that no configuration exists and automatically opens the **Settings** view so you can enter your connection details. Once saved, the database is created and the app is ready to use.

Settings can be changed at any time through the **Settings** view; changes take effect immediately without restarting.

> ⚠️ **Security:** Credentials are stored in plaintext in `config.db`. Do **not** commit it — add it to `.gitignore`.

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
│   │   └── native-bridge.ts      # window.hasConfig / doDelete / saveImapConfig / loadImapConfig
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
└── 🗄️ config.db                  # Runtime SQLite database — do not commit
```

---

## 🤝 Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for architecture overview, how to add a new mail client, how to add a JS binding, frontend dev workflow, and code style guidelines.
