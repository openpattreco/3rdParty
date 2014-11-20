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
prog_name=rdimgwh
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
log_file=$prog_name.log
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench

cd $test_path

if [ -f $bench_path/$prog_name"_exectime.log" ]
then
  rm $bench_path/$prog_name"_exectime.log"
fi


run_test () {
    image_descr=$1
    expected_result=$2
    args=$3
    cmd="$program $args";
echo --------------------------------------------------------------------------------
echo "Image Type: ""$image_descr"
echo "Expected Result: ""$expected_result"
echo ARGS: $args
echo
echo $cmd
echo
START=$(date +%s)
$cmd
END=$(date +%s)
. $bench_path/exec_time.sh
calc_time $prog_name "$cmd" $START $END

}

echo "Expect a usage message, and three: ""'ERROR : get_wh : raw or unknown image type'"

# usage
run_test "no input file" "usage message expected"  ""

# usage
run_test "no input file" "version message expected"  "-version"

# RAW: no explicit size within image data
run_test "raw fingerprint" \
         "raw image error" \
         "../../data/finger/gray/raw/finger.raw"

run_test "ANSI/NIST file with two raw images in records 3 and 5" \
         "two raw image errors" \
         "../../../an2k/data/nist.an2"

# WSQ
run_test "WSQ finger"  "500 x 500"  "../../data/finger/gray/wsq/finger.wsq"

run_test "ANSI/NIST file with WSQ images, selecting fingers 1 and 2" \
	       "records 2 and 3, both 804 x 752" \
	       "-f 1,2 ../../../an2k/data/a001-seg.an2"


# JPEGB
run_test "grayscale JPEGB face" \
         "768 x 1024" \
	       "../../data/face/gray/jpegb/face.jpb"

run_test "color JPEGB face" \
         "768 x 1024" \
	       "../../data/face/rgb/jpegb/face.jpb"


run_test "ANSI/NIST file with JPEGB images, selecting IDCs 0 and 1" \
	       "records 2 and 3, both 804 x 752" \
	       "-n 0,1 ../../../an2k/data/a001_jpb.an2"


# JPEGL
run_test "grayscale JPEGL finger"  "500 x 500" \
         "../../data/finger/gray/jpegl/finger.jpl"


run_test "color JPEGL face"  "768 x 1024" \
	       "../../data/face/rgb/jpegl/face.jpl"

run_test "ANSI/NIST file with JPEGL images, selecting index fingers" \
	       "records 3 and 8, 804 x 752 and 804 x 748" \
	       "-f index ../../../an2k/data/a001_jpl.an2"


# JP2
run_test "color JP2 face"  "768 x 1024" \
	       "../../data/face/rgb/jp2/face.jp2"

run_test "ANSI/NIST file with JP2 images, selecting plain impressions" \
	       "records: 12, 13, 14, 15, heights: 1000, widths: 1608, 412, 392, 1572" \
	       "-i plain ../../../an2k/data/a001_jpl.an2"


# JP2L
run_test "color JP2L face"  "768 x 1024" \
	       "../../data/face/rgb/jp2/face.jp2l"

run_test "ANSI/NIST file with JP2L images, selecting rolled thumbs" \
	       "records 2 and 7, 804 x 752 and 804 x 748" \
	       "-f thumb:rolled ../../../an2k/data/a001_jpb.an2"


# PNG
run_test "color PNG face"  "768 x 1024" \
	       "../../data/face/rgb/png/face.png"

run_test "ANSI/NIST file with PNG images, selecting left middle finger" \
	       "record 9, 804 x 748" \
	       "-f lmf ../../../an2k/data/a001_jpb.an2"

echo --------------------------------------------------------------------------------

diff -s rdimgwh.log rdimgwh_nist.log
