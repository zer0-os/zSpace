#!/bin/bash
if ! cd "$1"
then
  echo "Error during changing directory "
  read -rsp $'Press any key to continue...\n' -n1 key;
  exit
fi
if ! chmod u+x 'mvnw'
then
  echo "Error during making mvnw file executable "
  read -rsp $'Press any key to continue...\n' -n1 key;
  exit
fi
if ! ./mvnw org.web3j:web3j-maven-plugin:4.5.11:generate-sources
then
  echo "Error during contracts compilation "
  read -rsp $'Press any key to continue...\n' -n1 key;
  exit
fi