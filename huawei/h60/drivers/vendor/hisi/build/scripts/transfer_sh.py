#!/usr/bin/python
#-*- coding: UTF-8 -*-
#**************************************************************
#
#          ��Ȩ���� (C), 2001-2012,��Ϊ�������޹�˾
#
#**************************************************************
#�ļ���    transfer_sh.py
#�汾��    ������
#����      ��l00167020
#��������  ��2013��08��05��
#��������  ����������ѹ�������䵽����
#ʹ�÷���  : 
#�������  : 
#�������  ��
#����ֵ    ��
#�޸���ʷ  ��
#1.����    ��2013��08��05��
#  ����    ��l00167020
#  �޸����ݣ������ļ�

import os
import sys

def compress_and_transfer(top_dir, branch_name, product_name, buildowner, compiletime,additional_pkg_name):
	# transfer target ,transer target path
	trans_target_name = branch_name+'_'+product_name+'_'+ buildowner+'_'+compiletime 
	trans_target_path = top_dir +'/build/delivery' + '/' + product_name

	# create product directory in transer target directory
	trans_product_name = product_name
	if additional_pkg_name != '':
		trans_product_name = product_name + '_' + additional_pkg_name
	
	# first compress
	compress_target_layer_1 = 'lib/' + ' ' + 'log/' + ' ' + 'img/' +' ' + 'tool/' + ' ' + 'image/'+ ' ' + 'buildinfo.txt' 		
	compress_product_name = trans_product_name + '.rar'
	if os.path.exists(trans_target_path + '/' + compress_product_name):
		os.system('rm -f' + ' ' + trans_target_path + '/'+ compress_product_name)

	cmd = 'cd ' + trans_target_path + '&& zip -r1q ' + trans_target_path + '/'  + compress_product_name + ' ' + compress_target_layer_1
	errcode = os.system(cmd)
	if errcode != 0:
		print "compress_and_transfer :  compress product failed!"
		return errcode
		
	# second compress
	compress_target_layer_2 = compress_product_name + ' ' + 'timestamp.log' + ' ' + 'buildowner.txt'	
	compress_target_name = trans_target_name + '.rar'	
	if os.path.exists(trans_target_path + '/' + compress_target_name):
		os.system('rm -f' + ' ' + trans_target_path + '/'+ compress_target_name)
	
	cmd = 'cd ' + trans_target_path + '&& zip -r1q ' + trans_target_path + '/' + compress_target_name + ' ' + compress_target_layer_2
	errcode = os.system(cmd)
	if errcode != 0:
		print "compress_and_transfer :  compress failed!"
		return errcode
		
	# transfer 
	trans_scripts_path = top_dir + '/build'
	errcode	= os.system('cd ' + trans_scripts_path+ ' && bash hibuild_upload.sh '+ trans_target_path + ' '+ compress_target_name)
	if errcode != 0:
		print "compress_and_transfer :  transfer failed!"
		return errcode
	
	# delete rar after transfer complete 
	os.system('rm -f' + ' ' + trans_target_path + '/' +compress_product_name)
	os.system('rm -f' + ' ' + trans_target_path + '/'+ compress_target_name)
	
	return 0	
	