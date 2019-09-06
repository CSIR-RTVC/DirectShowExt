find_package(baseclasses 1.0 REQUIRED)

# source files for BandwidthMonitor
SET(SRCS 
    main.cpp
)

ADD_EXECUTABLE(TestApp 
	${SRCS}
)

target_link_libraries(TestApp 
	BaseClasses::BaseClasses
)

 