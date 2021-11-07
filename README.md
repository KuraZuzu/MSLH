# MSLH [Micromouse Standard Library for HALDriver]

This is a library for mobile robots using HALDriver With c++ <br>
Please refer to the following repository(https://github.com/KuraZuzu/MSLH.git) for instructions.<br>
<br>
[author: Kazushi Kurasawa (KuraZuzu)]

You need generate code from .ioc(STM32CubeMX) project. <br>
[.ioc version : 6.2.1]

これはマイクロマウス向けのc/c++プログラムであり，HALドライバを用いています．
.ioc ファイル(STM32CubeMX) によるピンや内部クロックの設定から生成されたコードを必要とします．このプロジェクトではCmakeに向けて解説します． <br>
また，説明のために "MSLH" が "ZuzuHalfTPPmod1" というProjectに内包されているものと仮定します． <br>
<br>
CMSIS-DSPライブラリがSTM32Cubeの "/STM32Cube/Repository/<現在のファームウェアバージョン(例:STM32Cube_FW_F4_V1.26.1)>/Drivers/CMSIS/DSP/" に存在します．<br>
この中の "Include" と "Source" をコピーして，プロジェクトの"Drivers/CMSIS/DSP(なければ作る)" に配置し，コンパイルの設定に追加します．<br>
これは，CMakeLists.txt の include_directories() に "Drivers/CMSIS/DSP/Include"を追加すれば大丈夫です．

![instructions image](images/image_DirectoryStruct.png)

<br>
また，

```
add_definitions(-DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -D__FPU_PRESENT=1)
```

を

```
include_directories(Inc Drivers/STM32F4xx_HAL_Driver/Inc Drivers/STM32F4xx_HAL_Driver/Inc/Legacy Drivers/CMSIS/Device/ST/STM32F4xx/Include Drivers/CMSIS/Include Drivers/CMSIS/DSP/Include)
```
の後に追加します．
<br>
FPUを有効化するために以下の2点の静的ライブラリも加えます．<br>
- arm_cortexM4lf_math.lib
- libarm_cortexM4lf_math.a <br>

これは，"/STM32Cube/Repository/<現在のファームウェアバージョン>/Drivers/CMSIS/Lib"の中の "./ARM" と "./GCC" 内にあります<br>


これらをリンクする方法として
<br>
```
target_link_libraries(${PROJECT_NAME}.elf ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Lib/ARM/arm_cortexM4lf_math.lib ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Lib/GCC/libarm_cortexM4lf_math.a)
```
を
```
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT} )  <br>
```
の後に加えてください．
![instructions image](images/image_For_FPUSetting.png)

このリポジトリのプロジェクト設定等は KuraZuzu の機体向けのものであるため，自分で設定した .ioc ファイルで"Generate Code"による上書きを行えば自動で更新してくれます．
ただし，動作確認をしたのは Jetbrain社 の IDE である CLion のみです．

※ "Generate Code"の際には，main関数のコールバックを呼ばない設定にしてください.各設定ファイルが全て main.c に生成されてしまいます．(2021/02/14 現在)

![instructions image](images/image_GenerateCode_STM32CubeMX.png)
![instructions image](images/image_GenerateCodeSetting_STM32CubeMX.png)
![instructions image](images/image_SettingPins_STM32CubeMX.png)
