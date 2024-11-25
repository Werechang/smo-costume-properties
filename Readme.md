# smo-costume-properties
A Super Mario Odyssey mod that allows you to attach custom animations and custom player constants to a costume.
(ex: giving space suit moon gravity, or add animal costumes with custom animations)

## Usage
This mod introduces two archives:
- ObjectData/CostumeAnimations.szs for custom animations
- ObjectData/CostumePlayerConst.szs for player const modifications

You have to create them by yourself.
### CostumeAnimations
You can replace the costume animations by adding a byml file with the (internal) name of your costume.
This byml has a string property "Name" at the root of the file, which contains the name of the animation archive located in ObjectData.
Example:
```
CostumeAnimations.szs:
 - Mario64.byml
 - MyOwnNiceCostume.byml
```

Mario64.byml (with the default animation file ObjectData/PlayerAnimation.szs):
```yaml
root:
  - Name: "PlayerAnimation"
```


MyOwnNiceCostume.byml (with the animation file ObjectData/MyOwnPlayerAnimationFile.szs):
```yaml
root:
  - Name: "MyOwnPlayerAnimationFile"
```
### CostumePlayerConst
This works pretty much the same as animations. \
You can apply the costume's player const by adding a byml file with the (internal) name of your costume.
The byml file works exactly like a normal PlayerConst.byml (PlayerActorHakoniwa/PlayerConstMoon.byml for reference). \
**Important**: This does not apply if you are on the moon!
You have to create a `[CostumeName]Moon.byml` for that! \
Example:
```
CostumeAnimations.szs:
 - Mario64.byml
 - MyOwnNiceCostume.byml
 - MyOwnNiceCostumeMoon.byml
```

### Notes
1. Issues? Read the readme again
2. Make sure that your archives are not corrupted.
3. Enabling logging gives you hints on what you might have missed.
4. The mod will resort to default animations and player const if nothing was found.

## Building
The only supported host platforms are Windows and Linux.

Dependencies: [CMake](https://cmake.org/)(>=v3.21) and [Ninja](https://ninja-build.org/).  
Both should be available from your package manager, be it `winget`, `apt`, `pacman` or anything else.

To configure the CMake project, run this command. See [definitions](#CMake definition args) for flags to add on.
```shell
cmake -S . -B build -G Ninja --toolchain=cmake/toolchain.cmake
```
And to build using Ninja, simply run:
```shell
ninja -C build
```
Any other target generators are untested and unsupported.

## Logging
As you can see in user/src/main.cpp, there's a few logger destinations available.
The default file location in that file is `sd:/mallow.log`.
If you want network logging, you can make a json file at `sd:/mallow.json` with the contents:
```json
{
  "logger": {
    // The address (hostname or IPv4) where the log server is hosted.
    "ip": "XXX.XXX.XXX.XXX",
    // The default port of choice is 3080.
    "port": 3080,
    // Reconnect when a server could not be found.
    "reconnect": false
  }
}
```


### CMake definition arguments:
- `-DFTP_IP=XXX.XXX.XXX.XXX`
  - optional, will enable FTP deployment builds if specified 
  - your switch's IP for deployment with the `subsdk9_deploy_ftp` target
- `-DFTP_PORT=5000`, `-DFTP_USERNAME=anonymous`, `-DFTP_PASSWORD=anonymous`
  - optional, uses defaults above
  - your switch's FTP port, username, and password
- `-DFTP_TITLE_ID=0100000000010000`
  - optional, uses defaults above
  - what title id to use when placing deployments into `/atmosphere/contents/FTP_TITLE_ID/exefs/`

## Credits
- [shadowninja108](https://github.com/shadowninja108)
  - [exlaunch](https://github.com/shadowninja108/exlaunch)
  - Lots of love <3
- [EngineLessCC](https://github.com/EngineLessCC)
  - [exlaunch-cmake](https://github.com/EngineLessCC/exlaunch-cmake/)
- [CraftyBoss](https://github.com/CraftyBoss)
  - [SMO-Exlaunch-Base](https://github.com/CraftyBoss/SMO-Exlaunch-Base/)
- [Atmosphère contributors](https://github.com/Atmosphere-NX/Atmosphere/graphs/contributors)
  - [Atmosphère](https://github.com/Atmosphere-NX/Atmosphere/)
- [marsmallow](https://github.com/odyssey-modding/marshmallow)
- [Thog](https://github.com/Thog)
  - [oss-rtld](https://github.com/Thog/oss-rtld)
------------------------------
