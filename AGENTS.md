# AGENTS.md

## C++ Standard

This project uses **C++20**.

## Coordinate System Convention

* **+X**: Right
* **+Y**: Up
* **+Z**: Toward the camera (out of the screen)
* **–Z**: Camera viewing direction (into the scene)

## Build & Run

```bash
git submodule update --init --recursive .
mkdir build && cd build
cmake ..
make
```

Run the renderer with:

```bash
./ray_tracing path/to/config.yaml
```

⚠️ **Note:** Rendering can be extremely time-consuming (potentially hours on a powerful desktop). Do **not** attempt to run it in a sandbox environment. Compiling alone is sufficient to check for build errors.

## Project Structure

- **main.cpp**: Entry point
- **config/**: Configuration files
- **assets/**: Assets
- **src/**: Source implementations
- **include/**: Header files
- **third_party/**: Third-party dependencies (use APIs only, don't modify or read source)

## Code Style Guidelines

As a code-generation assistant, follow these rules when modifying the code:

1. **Scoped Modifications Only**
   Apply changes *only* to the requested section. Do not modify unrelated parts, even if they appear suboptimal or incomplete. Preserve the existing logic.

2. **Concise Comments**
   Add comments only when strictly necessary to clarify complex or non-obvious code. Avoid redundant explanations for straightforward logic.

3. **Focus on Essentials ("Let It Crash")**
   Implement the requested functionality directly and efficiently. Do **not** add error handling, exception handling, or boundary checks unless explicitly instructed. Unexpected inputs or states should be allowed to fail naturally.


## Others

- Avoid `std::shared_ptr` except when it greatly simplifies lifetime management and does not hurt performance.
- Code comments should be written in English, unless I explicitly require the agent to use another language (Chinese).