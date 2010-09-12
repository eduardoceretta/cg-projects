
void main()
{
   gl_TexCoord[0] = gl_MultiTexCoord0; //triangleInfo
   gl_Position = ftransform();
}
