<p align="center">  
    <img src="https://tools.etdon.com/placeholder-image/generate?width=830&height=207&background-color=8B5DFF,6A42C2,563A9C&text=loadlib-dll-injector&text-color=FFFFFF" width=830 height=207>    
</p>

## 🔰 Introduction

The `loadlib-dll-injector` project is a DLL injector that's utilizing the well-known LoadLibary/CreateRemoteThread method to inject DLLs into other processes.

## 🚀 Getting Started

> [!IMPORTANT]
> Requirements:
> - Windows

If want to download a pre-compiled executable you can find all releases paired with their checksums here: [Releases](https://github.com/etdon/loadlib-dll-injector/releases)

To inject a DLL into a process simply run the program like this:
```
dll-injector.exe <Target> <DLL path>
```
The `target` parameter can either be the PID or the process name (e.g. `notepad.exe`) of the target process.

## 📦 Building
In order to compile the source file you can use any C compiler e.g. GCC:
```
gcc dll-injector.c -o dll-injector
```

## 🫴 Contributing
The contribution guidelines are a part of the `shared-guidelines` repository and can be found here: [Contributing][contributing]

## 📄 License
The `loadlib-dll-injector` project is licensed under the [Apache 2.0 License][license].
```
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

[contributing]: https://github.com/etdon/shared-guidelines/blob/main/CONTRIBUTING.md
[license]: https://github.com/etdon/loadlib-dll-injector/blob/master/LICENSE
