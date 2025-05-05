# AI generated (vibe coded) GEGL plugins

Grok and DeepSeek can make GEGL plugin from scratch and I may modify them but Grok and DeepSeek gets the credit here. This repo contains the source code of all AI generated GEGL plugins and my modifications to them which does include a few normal plugins of mine that they depend on. Grok and DeepSeek actually use **pixel modifying math*** to make GEGL plugins where as I only chain existing GEGL nodes, this allows them to do highly unique and specialized task that node chaining can't do. 

![image](https://github.com/user-attachments/assets/1656e73c-e7a1-494e-b173-30b56c6d40f5)

![image](https://github.com/user-attachments/assets/3a2c1096-b41e-4004-9e5a-b2957c11f688)


LOCATION TO PUT GEGL PLUGIN BINARIES 

**Windows**

 C:\Users\(USERNAME)\AppData\Local\gegl-0.4\plug-ins
 
 **Linux** 

 ~/.local/share/gegl-0.4/plug-ins

**Linux (Flatpak includes Chromebook)**

~/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins

then restart GIMP and go to "GEGL operation" to find the new plugins or if on 2.99.19 look in the menu

or look in 
 Filters>AI GEGL
