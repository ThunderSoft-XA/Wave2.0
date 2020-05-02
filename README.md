# Wave2.0
# Adreno-Wave Demo


Basic Information:


Maintainer: Kou Zhiwu、YongRong、YanShiqin、ZhangXu.


Start date: November 10th, 2019.


Function description:

Develop a demo application using the Adreno GPU SDK to display wave models.



Document introduction:

===>Assets
    |
    |--Fonts:
         |--Tuffy12.pak:       Font pak resource file.
    |--Textures：
         |--Logo.pak:          Texture pak resource file.


===>Android
    |
    |--InstallAssets.bat:      Copy assets resource files.
    |--InstallAssets.sh：      Copy assets resource files.
    |--AndroidManifest.xml：   AndroidManifest file.
    |--jni：
         |--Android.mk:        Android make file.
         |--Application.mk:    Application make file.


===>glm:   OpenGL Mathematics(GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.

===>wave.cpp mesh.cpp gl_aux.cpp:   Wave source code cpp file.

===>wave.h mesh.h gl_aux.h 
===>linmath.h math_aux.h:   Wave source code header file.

===>Scene.cpp:   Main scene cpp file.

===>Scene.h:   Main scene header file.



Usage Instructions:

1、Download code from github according to the repository from　“https://github.com/ThunderSoft-XA/Wave2.0.git”.

2、Install Adreno GPU SDK v5.0.

3、Install Android NDK r17b.

4、Install Apache Ant 1.9.14.

5、Install Snapdragon Profiler.

6、Compile APK source code and install it to android device.

7、Open the APK, and you will enjoy it.

8、If you care about some performance indicators, you can use Snapdragon Profiler to view it.

