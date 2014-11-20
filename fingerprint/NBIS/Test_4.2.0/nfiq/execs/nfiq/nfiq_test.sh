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

package=nfiq
prog_name=nfiq
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench


cd $test_path

if [ -f $bench_path/$prog_name"_exectime.log" ]
then
  rm $bench_path/$prog_name"_exectime.log"
fi

run_test ()
{
  input_file=$1
  options=$2
    
  cmd="$program $options ../../data/$input_file"
  START=$(date +%s)
  $cmd >> nfiq.log
  END=$(date +%s)
  . $bench_path/exec_time.sh
  calc_time $prog_name "$cmd" $START $END
}


run_nqm_test () {
  # This test runs nfiq with both input and output files, causing it
  # to convert image records type 4 to type 14, if necessary, and to
  # insert NQM fields.

  input_file=$1
  output_file=$2
  reference_file=$output_file
  reference_file=~s/\.an2/-nist.an2/
    
  cmd="$program ../../data/$input_file $output_file"
  START=$(date +%s)
  $cmd >> nfiq.log
  END=$(date +%s)
  . $bench_path/exec_time.sh
  calc_time $prog_name "$cmd" $START $END
}

> nfiq.log         	# create an empty log file

#run_test "input file" "options"
run_test a011r_02.wsq -d
run_test a011r_03.wsq -d
run_test a011r_04.wsq -d
run_test a011r_05.wsq -d
run_test a011p_02.wsq -d
run_test a011p_03.wsq -d
run_test a011p_04.wsq -d
run_test a011p_05.wsq -d

run_test a011r_02.wsq -v
run_test a011r_03.wsq -v
run_test a011r_04.wsq -v
run_test a011r_05.wsq -v
run_test a011p_02.wsq -v
run_test a011p_03.wsq -v
run_test a011p_04.wsq -v
run_test a011p_05.wsq -v

#run_nqm_test "input file" "output file"
run_nqm_test  a001.an2     a001-nqm.an2
run_nqm_test  a001-iaf.an2 a001-iaf-nqm.an2

run_test a001.an2      "-f rif"
run_test a001-iaf.an2  "-i rolled"
run_test a001.an2      "-n 3"
run_test a001.an2      "-t 14 -f 1"

#the following cmd creates 2 files in the ../../data dir:
#  a011r_02.ncm -and- a011r_02.raw
cmd="$prog_path""/dwsq raw ../../data/a011r_02.wsq -raw_out"
START=$(date +%s)
$cmd >> nfiq.log
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

run_test a011r_02.raw "-v -raw 568,600,8"

rm ../../data/a011r_02.raw
rm ../../data/a011r_02.ncm
