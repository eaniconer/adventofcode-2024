#!/usr/bin/env bash


if [ -n "$1" ] && [[ "$1" =~ ^[0-9]+$ ]]; then
  if [ ! -d "day-$1" ]; then
    cp -fr day-template day-$1
    sed -i "s/template/$1/g" day-$1/CMakeLists.txt
    echo "add_subdirectory(day-${1})" >> CMakeLists.txt
  else
    echo "Directory day-$1 already exists. Skipping."
  fi
else
  # $1 is invalid
  echo "Invalid input. Please enter a number."
  exit 1
fi



