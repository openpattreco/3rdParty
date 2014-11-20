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

package=mindtct
prog_name=mindtct
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench


run_test () {

  echo ----------------------------------------
  echo ----------------------------------------
  cd $test_path/$1;

  if [ -f $bench_path/$prog_name"_"$1"_exectime.log" ]
  then
    rm $bench_path/$prog_name"_"$1"_exectime.log"
  fi

  file_name=$2
  input_file=$file_name.eft  # concatenate
  #postfix_file=$file_name""_nist  
  cmd="$program ../../../data/$input_file $file_name"
  echo $cmd
  echo
  START=$(date +%s)
  $cmd
  END=$(date +%s)
  . $bench_path/exec_time.sh
  newname=$prog_name"_"$1
  calc_time $newname "$cmd" $START $END
}


#run_test "dir" "input file root name"
run_test g001 g001t2u
run_test g002 g002t3u
run_test g003 g003t8u
run_test g004 g004t8u
run_test g005 g005t8u
run_test g006 g006t6u
run_test g007 g007t1u
run_test g008 g008t6u
run_test g009 g009t8u
run_test g011 g011t7u


