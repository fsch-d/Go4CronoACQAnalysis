# Go4 user analysis
This is a go4 (https://go4.gsi.de) user analysis using the data acquired with cronoacq (https://github.com/fsch-d/cronoacq) 


# Installation on Debian (Ubuntu)

## required apt packages:
```rb
sudo apt-get install dpkg-dev cmake g++ gcc binutils libx11-dev libxpm-dev libxft-dev libxext-dev libssl-dev && \
sudo apt-get install git qt6-base-dev libxkbcommon-dev libxkbfile-dev libfftw3-dev libfftw3-doc python3-pip freeglut3-dev
sudo apt-get install libqt6webenginecore6 libqt6webenginewidgets6 qt6-webengine-dev
```
## ROOT install (from source)
```rb
git clone --branch latest-stable --depth=1 https://github.com/root-project/root.git root_src  && \
mkdir root_build root_install && cd root_build  && \
cmake -Dwebgui=ON -Dqt6web=ON -Droot7=ON -Dbuiltin_xrootd=ON -DCMAKE_CXX_STANDARD=17 -DCMAKE_INSTALL_PREFIX=../root_install ../root_src  && \# && check cmake configuration output for warnings or errors
cmake --build . -- install -j4  && \ # if you have 4 cores available for compilation
source ../root_install/bin/thisroot.sh # or thisroot.{fish,csh}
```
## GO4 install (from source)
```rb
git clone --branch master --depth=1 https://github.com/gsi-ee/go4.git go4_src  && \
mkdir go4_build go4_install && cd go4_build  && \
cmake -Dqt6=ON -Dhdf5=ON -DCMAKE_INSTALL_PREFIX=../go4_install ../go4_src  && \ # && check cmake configuration output for warnings or errors
cmake --build . -- install -j4  && \ # if you have 4 cores available for compilation
source ../go4_install/go4login
```
## This user analysis
```rb
git clone --branch master --depth=1 https://github.com/fsch-d/Go4CronoACQAnalysis.git Go4CronoACSAnalysis
cmake .
make all
```
## Comments
To automatically load the environment variables, you can add the following line to your .bashrc file:
```rb
source /PATH_TO_YOUR_GO4_DIRECTORY/go4_install/go4login
```

On Wayland, some of the widgets in the GUI might glitch. This should not happen if go4 is started with:
```rb
go4 --web
```




