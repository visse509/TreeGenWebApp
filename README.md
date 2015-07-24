# TreeGenWebApp
This project is divided into two parts.
The first part is a C++ backend application.
The purpose of the application is to procedurally generate 3d-models of trees.
The application compiles into an .exe file which will be used by the second part.

The second part is a front-end application implemented using C# in MVC .NET.
The webapplication enables a user to control how the trees will be generated, as well as showing the final result in the browser.

The interaction between the two parts is implemented using .xml files to determine the generation of the trees, and .fbx for the tree models.
The webapplication will execute the backend application with a .xml file as a parameter, the tree will be generated according to the .xml file and the output is the .fbx file.

