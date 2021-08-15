# Dependencies
- C++17 including Parallism (ie. gcc with tbb)
- libgtest
- optional: gnuplot to visualize data

# Steps to compile
0. Get a working linux. can be a vm or wsl or native. We will need Parallel STL support in gcc so Ubuntu 20.04 will work nicely.
1. Install all dependencies: apt-get update && apt-get install cmake g++ libgtest-dev libtbb-dev
2. Enter a working dir of your choice
2. Clone the repo: git clone https://bitbucket.org/antharion/theorysimulator.git
3. mkdir build && cd build
4. cmake ../theorysimulator
5. make

You should fine the executables in the bin folder. Most likely you want to use currentRates

# Steps to run via docker image
* Download docker desktop via https://www.docker.com/products/docker-desktop
* Clone the repo as shown above
* Run the command line in the theorysimulator/ folder
* Run the following commands:
  - docker-compose build (only needs to be ran once to build the docker image unless new code is pulled)
  - docker run -it theorysimulator
  - cd bin
* You should now be able to run the executables as described below

# Executables:
``./currentRates <sigma> <tau1> <tau2> <tau3> <tau4> <tau5> <tau6> <tau6> <tau7> <tau8>``  

* Gives you current tau/second for taus entered as commandline arguments based on simulation results in the current working directory
* The columns have the following meaning
  * The first two columns (Manual and auto) are the expected rate for the next publication of this theory starting at the given tau.
  * The third column is just the ratio between the first two.
  * The fourth and fifth column are the multiplier at which the system wants to publish
    * This might not yet be the ideal multiplier for you!
    * To get the ideal multiplier check if pushing a bit further is useful by using an expression like 3600*d(log10(tau)) in the prestige expression tester. Note that this expression only works while tau == rho
  * The sixth and seventh columns contain the names of the strategies used
  * Eighth and nineth column contain the duration in hours for the publication
* For manual play you are looking for a theory with high manual rate and high ratio
* For idle play you are  looking for a theory with high auto rate and low ratio

``./main``  

* its a gtest containing all simulation runs
* for each run it will write a file to current working dir
* probably best to do a full run intitially (may take a looong time), and afterwards redo theories you modified with --gtest_filter=

``./makePlotFiles``  

* assumes anything matching T[1-8] is an output of main
* creates gnuplot files per theory and overall
* also shows tau/hr for hardcoded taus

``./idealtau``  

* tries to create an ideal tau distribution
