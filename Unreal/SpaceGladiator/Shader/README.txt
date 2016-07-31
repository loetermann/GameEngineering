Copy GridShader.usf and MaterialTemplate.usf files to

"Program-Path"\Epic Games\4.10\Engine\Shaders

You might want to backup your original MaterialTemplate.usf first.

The Shader functions are used by Content/Materials/Grid_Mat.uasset. Some parameters are defined in Grid_Mat that you might need to adapt to your hardware as a lower resolution leads to more aliasing effects.
Find comments for the paramters in the Material Blueprint, which explain which paramters to increase/decrease for sharper lines/less aliasing.