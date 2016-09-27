# -*- coding: utf-8 -*-
import xlrd
import csv
from os import sys



COL_CHAINAGE    = 1
COL_ELEV        = 2

# File to write to...
file = open('tunnel_elevation.h','w')

# Excel spreadsheet to read elevation from...
workbook = xlrd.open_workbook('TBM Invert As Built Survey Data.xlsx')
all_worksheets = workbook.sheet_names()


file.write('/* INVERT ELEVATION OF 2ND TAILRACE TUNNEL */\n\n')
file.write('tunnel_elevation[961][2] = { \n')

# Get elevation from each worksheet in the excel file
for worksheet_name in all_worksheets:
    worksheet = workbook.sheet_by_name(worksheet_name)

    for rownum in list(xrange(worksheet.nrows))[7:]:

        chainage = ''.join(str(worksheet.cell(rownum, COL_CHAINAGE).value).split('+'))
        elevation = worksheet.cell(rownum, COL_ELEV).value


        file.write('\t{' + str(chainage) + ',' + str(elevation) + '},\n') # python will convert \n to os.linesep


    
file.write('};\n')
file.close()

