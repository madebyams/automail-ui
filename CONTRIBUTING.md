# 🤝 Contributing to Automail UI

## 🏗️ Architecture

The backend is split into four clear layers — keep them separate:

| Layer | Location | Responsibility |
|-------|----------|---------------|
| 🔌 **Entry point** | `src/main/main.cpp` | Wires all components together |
| 📬 **Mail** | `src/mail/` + `include/mail/` | Protocol clients and deletion logic |
| 🗄️ **Config** | `src/config/` + `include/config/` | Loading, validating, and persisting settings |
| 🖥️ **GUI** | `src/gui/` + `include/gui/` | WebView window, JS bindings, HTTP server |

> 📐 **Golden rule:** Business logic lives in C++. Presentation lives in Angular. The only bridge between them is the four functions on `window`.

---

## 📬 Adding a new mail client

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

## 🎨 Modifying the frontend

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

## 🔗 Adding a new JS binding

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

## 🖊️ Code style

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
