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

package=jpeg2k
prog_name=jp2codec
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench

cd $test_path

if [ -f $bench_path/$prog_name"_exectime.log" ]
then
  rm $bench_path/$prog_name"_exectime.log"
fi

if [ -f $prog_name.log ]
then
  rm $prog_name.log
fi

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/003_R02_nist.pnm -F ../../data/finger/gray/jp2/003_R02_fing-1000-lossy.jp2 --fing-1000-lossy"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/003_R02_nist.pnm -F ../../data/finger/gray/jp2/003_R02_fing-1000-lossy.jp2 --fing-1000-lossy" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/003_R02_nist.pnm -F ../../data/finger/gray/jp2/003_R02_fing-1000-lossless.jp2 --fing-1000-lossless"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/003_R02_nist.pnm -F ../../data/finger/gray/jp2/003_R02_fing-1000-lossless.jp2 --fing-1000-lossless" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/003_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/003_R02_custom_lossy.jp2 \
                     -T jp2 -O mode=real \
		     -O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370 \
	             -O rates=14,23,32,53,80,133,200,320,533"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/003_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/003_R02_custom_lossy.jp2 \
-T jp2 -O mode=real \
-O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370 \
-O rates=14,23,32,53,80,133,200,320,533" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/003_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/003_R02_custom_lossless.jp2 \
                     -T jp2 -O mode=int \
		     -O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/003_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/003_R02_custom_lossless.jp2 \
-T jp2 -O mode=int \
-O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/jp2/003_R02_fing-1000-lossy.jp2 -F ../../data/finger/gray/pnm/003_R02_decoded_fing-1000-lossy.pnm"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/jp2/003_R02_fing-1000-lossy.jp2 -F ../../data/finger/gray/pnm/003_R02_decoded_fing-1000-lossy.pnm" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/jp2/003_R02_fing-1000-lossless.jp2 -F ../../data/finger/gray/pnm/003_R02_decoded_fing-1000-lossless.pnm"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/jp2/003_R02_fing-1000-lossless.jp2 -F ../../data/finger/gray/pnm/003_R02_decoded_fing-1000-lossless.pnm" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END


echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/007_R02_nist.pnm -F ../../data/finger/gray/jp2/007_R02_fing-1000-lossy.jp2 --fing-1000-lossy"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/007_R02_nist.pnm -F ../../data/finger/gray/jp2/007_R02_fing-1000-lossy.jp2 --fing-1000-lossy" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/007_R02_nist.pnm -F ../../data/finger/gray/jp2/007_R02_fing-1000-lossless.jp2 --fing-1000-lossless"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/007_R02_nist.pnm -F ../../data/finger/gray/jp2/007_R02_fing-1000-lossless.jp2 --fing-1000-lossless" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/007_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/007_R02_custom_lossy.jp2 \
                     -T jp2 -O mode=real \
		     -O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370 \
	             -O rates=14,23,32,53,80,133,200,320,533"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/007_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/007_R02_custom_lossy.jp2 \
-T jp2 -O mode=real \
-O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370 \
-O rates=14,23,32,53,80,133,200,320,533" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/pnm/007_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/007_R02_custom_lossless.jp2 \
                     -T jp2 -O mode=int \
		     -O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/pnm/007_R02_nist.pnm -t pnm -F ../../data/finger/gray/jp2/007_R02_custom_lossless.jp2 \
-T jp2 -O mode=int \
-O numdecomlvls=6 -O prg=rpcl -O hrcn=39370 -O vrcn=39370" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/jp2/007_R02_fing-1000-lossy.jp2 -F ../../data/finger/gray/pnm/007_R02_decoded_fing-1000-lossy.pnm"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/jp2/007_R02_fing-1000-lossy.jp2 -F ../../data/finger/gray/pnm/007_R02_decoded_fing-1000-lossy.pnm" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

echo ----------------------------------------
echo ----------------------------------------
cmd="$program -f ../../data/finger/gray/jp2/007_R02_fing-1000-lossless.jp2 -F ../../data/finger/gray/pnm/007_R02_decoded_fing-1000-lossless.pnm"
echo $cmd
echo "$prog_name -f ../../data/finger/gray/jp2/007_R02_fing-1000-lossless.jp2 -F ../../data/finger/gray/pnm/007_R02_decoded_fing-1000-lossless.pnm" >> $prog_name.log
START=$(date +%s)
 $cmd >> $prog_name.log 2>&1
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

