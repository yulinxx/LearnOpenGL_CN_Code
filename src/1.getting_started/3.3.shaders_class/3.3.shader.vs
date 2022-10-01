// 顶点着色器（Vertex Shader）

#version 330 core

// 布局限定符 基本语法为：
// layout(qualifier1​, qualifier2​ = value, ...) variable definition
// 顶点着色器要从CPU端要数据，比起其他着色器，就有 layout
// layout和CPU代码中的glVertexAttribPointer有关
// layout（location=1） 与代码中 glVertexAttribPointer 函数第一个参数指定的序号是一致的

layout (location = 0) in vec3 aPos;  // 位置变量的属性位置值为0
layout (location = 1) in vec3 aColor;  // 位置变量的属性位置值为1


out vec3 ourColor;

// 着色器是使用一种叫GLSL的类C语言写成的。GLSL是为图形计算量身定制的，它包含一些针对向量和矩阵操作的有用特性。
// 着色器的开头总是要声明版本，接着是输入和输出变量、uniform和main函数。
// 每个着色器的入口点都是main函数，在这个函数中我们处理所有的输入变量，并将结果输出到输出变量中。

void main()
{
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}