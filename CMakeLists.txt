if(NOT CMAKE_PROJECT_NAME STREQUAL Go4)
   cmake_minimum_required(VERSION 3.9)
   project(Go4CronoACQSource)
   find_package(Go4)
   find_package(PkgConfig REQUIRED)
   pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)
   include_directories(PkgConfig::FFTW)
   link_libraries(PkgConfig::FFTW)   
   include(${GO4_USE_FILE})
   include_directories(${_go4inc_})
endif()

GO4_USER_ANALYSIS(
   LINKDEF
      CronoACQLinkDef.h
   HEADERS
      TCronoACQAnalysis.h
      TCronoACQAnlEvent.h
      TCronoACQAnlProc.h
      TCronoACQRawPar.h
      TCronoACQCorrPar.h
      TCronoACQMomCalcPar.h
      TCronoACQMomCorrPar.h
      TCronoACQUnpackEvent.h
      TCronoACQUnpackProc.h
      TCronoACQRawEvent.h
      TCronoACQEventSource.h
   SOURCES
      TCronoACQAnalysis.cxx
      TCronoACQAnlEvent.cxx
      TCronoACQAnlProc.cxx
      TCronoACQRawPar.cxx
      TCronoACQCorrPar.cxx
      TCronoACQMomCalcPar.cxx
      TCronoACQMomCorrPar.cxx
      TCronoACQUnpackEvent.cxx
      TCronoACQUnpackProc.cxx
      TCronoACQRawEvent.cxx
      TCronoACQEventSource.cxx
)
