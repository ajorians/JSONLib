PROJECT(JSONLib)

cmake_minimum_required(VERSION 2.6)

set(gtest_force_shared_crt ON)
ADD_SUBDIRECTORY(gtest-1.7.0)
ADD_SUBDIRECTORY(JSONLib)
ADD_SUBDIRECTORY(JSONLib_Tester)
