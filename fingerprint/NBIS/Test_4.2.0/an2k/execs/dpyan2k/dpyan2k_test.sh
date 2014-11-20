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

package=an2k
prog_name=dpyan2k
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench

cd $test_path
pwd

if [ -f $bench_path/$prog_name"_exectime.log" ]
then
  rm $bench_path/$prog_name"_exectime.log"
fi


run_test () {

  echo ----------------------------------------
  echo ----------------------------------------

  output_descr=$1
  input_file=$2
  options=$3

  echo "This checks "$output_descr

  cmd="$program $options ../../data/$input_file"
  echo $cmd
  START=$(date +%s)
  $cmd
  END=$(date +%s)
  . $bench_path/exec_time.sh
  newname=$prog_name
  calc_time $newname "$cmd" $START $END
}


#run_test "output_descr" "input file name" "options"
run_test "two raw grayscale fingerprint images, overlaid with \
several red squares marking ANSI/NIST minutiae, and do it verbosely" \
	 "nist.an2" "-v";

run_test "two raw grayscale fingerprint images, overlaid with \
several larger 10-pixel red squares marking IAFIS minutiae, in a wide window" \
	 "iafis.an2" "-I -p 10 -W 1000";

run_test "two WSQ grayscale fingerprint images, overlaid with \
rectilinear green boxes showing the segmentation data from a SEG field \
added by nfseg, in a window that is too narrow" \
	 "a001-seg.an2" "-f1,13 -W 450";

run_test "two WSQ grayscale fingerprint images, overlaid with \
green boxes showing the segmentation data from an ASEG field added by nfseg, \
in a short window" \
         "a001-aseg.an2" "-f2,14 -H 500";

run_test "one JPB grayscale right index finger, with top right corner at \
screen coordinates 200,200" \
	 "a001_jpb.an2"  "-f ri -X 200 -Y 200";

run_test "one JPL grayscale image with IDC 2: right middle finger, \
with a wide border" \
	 "a001_jpl.an2"  "-n 2 -b 10";

run_test "one JP2L grayscale right ring finger, in a tall window" \
	 "a001_jp2l.an2" "-f rr -H 1000";

run_test "one PNG grayscale rolled right little finger" \
	 "a001_png.an2" "-f rl:ro";

run_test "one PNG grayscale plain right thumb" \
	 "a001_png.an2" "-f rt -i pl";

run_test "one raw Type-13 latent print, save the selector in a file" \
	 "nist.an2" "-t vrli -w latent.sel ";

run_test "same as previous, using saved selector" \
	 "nist.an2" "-r latent.sel -T 'testing selector file'";

run_test "one RAW color face" "face_raw.an2" "" ;

run_test "one JPEGB color face" "face_jpb.an2" "";

run_test "one JPEGL color face" "face_jpl.an2" "";

run_test "one JP2 color face" "face_jp2.an2" "";

run_test "one JP2L color face" "face_jp2l.an2" "";

run_test "one PNG color face" "face_png.an2" "";
