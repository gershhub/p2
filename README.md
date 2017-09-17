This is a fork of github user [absmall's](https://github.com/absmall) [P^2 algorithm implementation](https://github.com/absmall/p2). I ported it from C++ to C and into a plugin for the Unity game engine. An xcode project with iOS and MacOS library targets is included, along with a C# plugin script for accessing the plugin from Unity. There is also a port of the test program to C#.

From absmall's README:

This program implements the P^2 algorithm as documented in "The P-Square Algorithm for Dynamic Calculation of Percentiles and Histograms without Storing Observations," Communications of the ACM, October 1985 by R. Jain and I. Chlamtac.

Both the point method and histogram method are implemented.

p2.cc implements the algorithm. main.cc is a test driver program. data.txt provides sample data to test the algorithm. It is sampled from a normal distribution with mean=0, standard deviation=1.
It was generated using R (http://www.r-project.org/) with the function "runif(200)".
