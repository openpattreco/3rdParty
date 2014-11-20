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

package=imgtools
prog_name=sd_rfmt
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench

cd $test_path
cp ../../data/sd_data/sd04.old .
cp ../../data/sd_data/sd09.old .
cp ../../data/sd_data/sd10.old .
cp ../../data/sd_data/sd14.old .
cp ../../data/sd_data/sd18.old .

if [ -f $bench_path/$prog_name"_exectime.log" ]
then
  rm $bench_path/$prog_name"_exectime.log"
fi


echo ----------------------------------------
echo ----------------------------------------
cmd="$program 4 jpl sd04.old"
echo $cmd
START=$(date +%s)
$cmd
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$prog_path/rdjpgcom sd04.jpl"
echo $cmd
START=$(date +%s)
$cmd > sd04.ncm
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program 9 jpl sd09.old"
echo $cmd
START=$(date +%s)
$cmd
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$prog_path/rdjpgcom sd09.jpl"
echo $cmd
START=$(date +%s)
$cmd > sd09.ncm
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program 10 jpl sd10.old"
echo $cmd
START=$(date +%s)
$cmd
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$prog_path/rdjpgcom sd10.jpl"
echo $cmd
START=$(date +%s)
$cmd > sd10.ncm
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program 14 wsq sd14.old"
echo $cmd
START=$(date +%s)
$cmd
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$prog_path/rdwsqcom sd14.wsq"
echo $cmd
START=$(date +%s)
$cmd > sd14.ncm
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program 18 jpl sd18.old"
echo $cmd
START=$(date +%s)
$cmd
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$prog_path/rdjpgcom sd18.jpl"
echo $cmd
START=$(date +%s)
$cmd > sd18.ncm
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

rm -f sd04.old
rm -f sd09.old
rm -f sd10.old
rm -f sd14.old
rm -f sd18.old
