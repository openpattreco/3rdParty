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
prog_name=chkan2k
prog_path=$TARGET_INSTALLATION_BIN_DIR
program=$prog_path/$prog_name
test_path=$NBIS_TEST_DIR/$package/execs/$prog_name
bench_path=$NBIS_TEST_DIR/bench

cd $test_path
pwd

if [ -f $bench_path/$prog_name"_bad_exectime.log" ]
then
  rm $bench_path/$prog_name"_bad_exectime.log"
fi



run_test () {

  echo ----------------------------------------
  echo ----------------------------------------

  output_descr=$1
  input_file=$2
  options=$3

  echo "This checks "$output_descr

  cmd="$program $options ../../data/bad/$input_file"
  echo $cmd
  START=$(date +%s)
  $cmd
  END=$(date +%s)
  . $bench_path/exec_time.sh
  newname=$prog_name"_bad"
  calc_time $newname "$cmd" $START $END
}


#run_test "output_descr" "input file name" "options"
run_test "one extra byte appended to the end of the file" a001-extra-byte.an2 ""

run_test "contents field contains one extra record, which does not exist." a001-extra-cnt.an2 "";

run_test "type-1 record missing from beginning of file." a001-headless.an2 "";

run_test "type-1 TOT field is 8 bytes, required size is 3 or 4 bytes." a001-item-too-big.an2 "";

run_test "type-1 TOT field is 2 bytes, required size is 3 or 4 bytes." a001-item-too-small.an2 "-c nist-samples.conf";

run_test "contents field list missing one entry, but the record exists." a001-missing-cnt.an2 "-c nist-samples.conf";

run_test "record missing one required field." a001-missing-field.an2 "-c nist-samples.conf";

run_test "field missing one required item." a001-missing-item.an2 "";

run_test "one finger mis-numbered" a001-one-dup.an2 "";

run_test "one finger missing" a001-one-missing.an2 "";

run_test "segment field missing an item" a001-seg-missing-item.an2 "";

run_test "file truncated at 999 bytes, in type-14 image data" a001-t14-data-trunc-999.an2 "";

run_test "type-14 record length specified one to small" a001-t14-len-1.an2 "";

run_test "type-14 record length specified one to large" a001-t14-len+1.an2 "";

run_test "type-14 record type in LEN field id, first char NULL, at byte 224" a001-t14-len-fid-null-224.an2 "";

run_test "type-14 record type in LEN field id, second char NULL, at byte 225" a001-t14-len-fid-null-225.an2 "";

run_test "type-14 record type in LEN field id truncated at 1st char, byte 224" a001-t14-len-fid-trunc-224.an2 "";

run_test "value of type-14 LEN field NULL at first char, byte 231" a001-t14-len-val-null-231.an2 "";

run_test "value of type-14 LEN field NULL at second char, byte 232" a001-t14-len-val-null-232.an2 "";

run_test "value of type-14 LEN field NULL at third char, byte 233" a001-t14-len-val-null-233.an2 "";

run_test "type-14 record terminator missing, at byte 42723" a001-t14-term-del-42723.an2 "";

run_test "type-14 record terminator NULL at byte 42723" a001-t14-term-null-42723.an2 "";

run_test "file truncated before type-14 record terminator at byte 42722" a001-t14-term-trunc-42722.an2 "";

run_test "type-1 record length field value one short" a001-t1-len-1.an2 "";

run_test "type-1 record length field value one long" a001-t1-len+1.an2 "";

run_test "type-1 record length field id, first char NULL" a001-t1-len-fid-null-0.an2 "";

run_test "type-1 record length field id, second char NULL" a001-t1-len-fid-null-1.an2 "";

run_test "type-1 record length field id, third char NULL" a001-t1-len-fid-null-2.an2 "";

run_test "type-1 record terminator missing, at byte 223" a001-t1-term-del-223.an2 "";

run_test "type-1 record terminator null, at byte 223" a001-t1-term-null-223.an2 "";

run_test "file truncated before type-1 record terminator, at byte 222" a001-t1-term-trunc-222.an2 "";

run_test "type-1 record contains two TOT fields, should have one and only one" a001-too-many-items.an2 "";

run_test "the file exists but its size is zero" empty.an2 "";

run_test "JP2 face image, invalid field 1.016 has value 'taboo'" face-forbidden-field.an2 "";

run_test "JP2 face with incorrect height in type-10 record" face-jp2-image-height.an2 "";

run_test "JP2 face with incorrect width in type-10 record" face-jp2-image-width.an2 "";

run_test "JP2 face with user defined field" face-udf.an2 "";

run_test "PNG face with invalid IMT value 'AFACE'" face-wrong-string.an2 "";

run_test "type-1 length field id number too large" vxt1-len-id-too-big.an2 "";

run_test "type-1 length field type id truncated" vxt1-len-type-eof.an2 "";

run_test "type-1 length field type id too large" vxt1-len-type-too-big.an2 "";

run_test "type-1 length value too large" vxt1-len-val-too-big.an2 "";