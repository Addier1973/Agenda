# AGENTS.md

## Cursor Cloud specific instructions

### Project overview

This repository contains two projects:

1. **Agenda Android App** – A contacts/agenda Android app (`com.cdp.agenda`) built with Java and Gradle. Uses SQLite for local storage, RecyclerView, Material Design, and AndroidX.
2. **Solar Calculator** – Standalone HTML/CSS/JS web calculator for photovoltaic solar panel sizing (`Calculadora_Solar_Web.html`). No build step needed.

### Environment requirements

- **JDK 17** (`openjdk-17-jdk-headless`) – Required for Android Gradle Plugin 7.4.2. JDK 21 (system default) is too new.
- **Android SDK** installed at `~/android-sdk` with platform 33, build-tools 33.0.2, and command-line tools.
- Environment variables are set in `~/.bashrc`:
  - `ANDROID_SDK_ROOT=/home/ubuntu/android-sdk`
  - `ANDROID_HOME=/home/ubuntu/android-sdk`
  - `JAVA_HOME=/usr/lib/jvm/java-17-openjdk-amd64`

### Build, test, and lint commands

| Command | Description |
|---|---|
| `./gradlew assembleDebug` | Build the debug APK (output: `app/build/outputs/apk/debug/app-debug.apk`) |
| `./gradlew test` | Run unit tests (JVM-based, no emulator needed) |
| `./gradlew lintDebug -x processDebugAndroidTestManifest` | Run lint checks (must skip androidTest manifest due to pre-existing `android:exported` issue in test dependencies targeting SDK 33) |

### Known issues

- `./gradlew lint` (without exclusion) fails because the `androidTest` manifest merge requires `android:exported` for test activities targeting API 31+. This is a pre-existing issue in the test dependency versions. Use the `-x processDebugAndroidTestManifest` workaround for lint.
- There is no Android emulator in the Cloud VM, so the app cannot be run end-to-end. Build verification and unit tests are the primary validation methods.

### Solar Calculator

The HTML calculator (`Calculadora_Solar_Web.html`) can be served with any HTTP server, e.g.:
```
python3 -m http.server 8080 --directory /workspace
```
Then open `http://localhost:8080/Calculadora_Solar_Web.html` in Chrome.
