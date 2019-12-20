@echo off
set SRC_DIR=%~dp0
set DST_DIR=%~dp0

protoc.exe -I=%SRC_DIR%message --cpp_out=%DST_DIR% StreamPlay.proto