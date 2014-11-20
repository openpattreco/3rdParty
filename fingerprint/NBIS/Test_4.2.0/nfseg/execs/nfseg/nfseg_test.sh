#!/bin/sh
#*******************************************************************************
#
# License: 
# This software and/or related materials was developed at the National Institute
# of Standards and Technology (NIST) by employees of the Federal Government
# in the course of their official duties. Pursuant to title 17 Section 105
# of the United States Code, this software is not subject to copyright
# protection and is in the public domain. 
#
# This software and/or related materials have been determined to be not subject
# to the EAR (see Part 734.3 of the EAR for exact details) because it is
# a publicly available technology and software, and is freely distributed
# to any interested party with no licensing requirements.  Therefore, it is 
# permissible to distribute this software as a free download from the internet.
#
# Disclaimer: 
# This software and/or related materials was developed to promote biometric
# standards and biometric technology testing for the Federal Government
# in accordance with the USA PATRIOT Act and the Enhanced Border Security
# and Visa Entry Reform Act. Specific hardware and software products identified
# in this software were used in order to perform the software development.
# In no case does such identification imply recommendation or endorsement
# by the National Institute of Standards and Technology, nor does it imply that
# the products and equipment identified are necessarily the best available
# for the purpose.
#
# This software and/or related materials are provided "AS-IS" without warranty
# of any kind including NO WARRANTY OF PERFORMANCE, MERCHANTABILITY,
# NO WARRANTY OF NON-INFRINGEMENT OF ANY 3RD PARTY INTELLECTUAL PROPERTY
# or FITNESS FOR A PARTICULAR PURPOSE or for any purpose whatsoever, for the
# licensed product, however used. In no event shall NIST be liable for any
# damages and/or costs, including but not limited to incidental or consequential
# damages of any kind, including economic damage or injury to property and lost
# profits, regardless of whether NIST shall be advised, have reason to know,
# or in fact shall know of the possibility.
#
# By using this software, you agree to bear all risk relating to quality,
# use and performance of the software and/or related materials.  You agree
# to hold the Government harmless from any claim arising from your use
# of the software.
#
#*******************************************************************************

package=nfseg
prog_name=nfseg
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench

# NOTE: redirection does not work if it is added to the end of the $cmd
# variable. That is why the redirection occurs on the line of actual
# command execution, eg, $cmd > filename.log

run_test () {

  echo ----------------------------------------
  echo ----------------------------------------
  input_file=$1
  #echo Input file: $input_file
  # copy the input file to curdir and rename
  cp ../../data/$1 a011r.wsq  #rename to a011r.wsq

  #strip the file extension
  log_file=`echo $input_file | sed 's/\(.*\)\..*/\1/'`
  #echo log_file: $log_file
  log_file=$log_file.log    #append the log extension
  options=$2
  cmd="$program $options a011r.wsq"
  echo $cmd
  START=$(date +%s)
  $cmd > $log_file
  END=$(date +%s)
  . $bench_path/exec_time.sh
  calc_time $prog_name "$cmd" $START $END

  rm -f a011r.wsq
}

cd $test_path

if [ -f $bench_path/$prog_name"_exectime.log" ]
then
  rm $bench_path/$prog_name"_exectime.log"
fi


#run_test  "input file"  "options" 
run_test   a011_02.wsq   "2 1 1 1 0"
run_test   a011_03.wsq   "3 1 1 1 0"
run_test   a011_04.wsq   "4 1 1 1 0"
run_test   a011_05.wsq   "5 1 1 1 0"
#run_test   a011_13.wsq   "13 1 1 1 0"


echo ----------------------------------------
echo ----------------------------------------

cp ../../data/a011_13.wsq a011p.wsq  #rename to a011p.wsq

cmd="$program 13 1 1 1 0 a011p.wsq"
START=$(date +%s)
$cmd > a011_13.log
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

rm -f a011p.wsq
