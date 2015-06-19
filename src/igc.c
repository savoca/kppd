/*
 * Copyright (c) 2015 savoca <savoca@codefi.re>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "kcal.h"
#include "msm_mdp.h"

static uint32_t igc_inverted[IGC_LUT_ENTRIES] = {
	267390960, 266342368, 265293776, 264245184,
	263196592, 262148000, 261099408, 260050816,
	259002224, 257953632, 256905040, 255856448,
	254807856, 253759264, 252710672, 251662080,
	250613488, 249564896, 248516304, 247467712,
	246419120, 245370528, 244321936, 243273344,
	242224752, 241176160, 240127568, 239078976,
	238030384, 236981792, 235933200, 234884608,
	233836016, 232787424, 231738832, 230690240,
	229641648, 228593056, 227544464, 226495872,
	225447280, 224398688, 223350096, 222301504,
	221252912, 220204320, 219155728, 218107136,
	217058544, 216009952, 214961360, 213912768,
	212864176, 211815584, 210766992, 209718400,
	208669808, 207621216, 206572624, 205524032,
	204475440, 203426848, 202378256, 201329664,
	200281072, 199232480, 198183888, 197135296,
	196086704, 195038112, 193989520, 192940928,
	191892336, 190843744, 189795152, 188746560,
	187697968, 186649376, 185600784, 184552192,
	183503600, 182455008, 181406416, 180357824,
	179309232, 178260640, 177212048, 176163456,
	175114864, 174066272, 173017680, 171969088,
	170920496, 169871904, 168823312, 167774720,
	166726128, 165677536, 164628944, 163580352,
	162531760, 161483168, 160434576, 159385984,
	158337392, 157288800, 156240208, 155191616,
	154143024, 153094432, 152045840, 150997248,
	149948656, 148900064, 147851472, 146802880,
	145754288, 144705696, 143657104, 142608512,
	141559920, 140511328, 139462736, 138414144,
	137365552, 136316960, 135268368, 134219776,
	133171184, 132122592, 131074000, 130025408,
	128976816, 127928224, 126879632, 125831040,
	124782448, 123733856, 122685264, 121636672,
	120588080, 119539488, 118490896, 117442304,
	116393712, 115345120, 114296528, 113247936,
	112199344, 111150752, 110102160, 109053568,
	108004976, 106956384, 105907792, 104859200,
	103810608, 102762016, 101713424, 100664832,
	99616240, 98567648, 97519056, 96470464,
	95421872, 94373280, 93324688, 92276096,
	91227504, 90178912, 89130320, 88081728,
	87033136, 85984544, 84935952, 83887360,
	82838768, 81790176, 80741584, 79692992,
	78644400, 77595808, 76547216, 75498624,
	74450032, 73401440, 72352848, 71304256,
	70255664, 69207072, 68158480, 67109888,
	66061296, 65012704, 63964112, 62915520,
	61866928, 60818336, 59769744, 58721152,
	57672560, 56623968, 55575376, 54526784,
	53478192, 52429600, 51381008, 50332416,
	49283824, 48235232, 47186640, 46138048,
	45089456, 44040864, 42992272, 41943680,
	40895088, 39846496, 38797904, 37749312,
	36700720, 35652128, 34603536, 33554944,
	32506352, 31457760, 30409168, 29360576,
	28311984, 27263392, 26214800, 25166208,
	24117616, 23069024, 22020432, 20971840,
	19923248, 18874656, 17826064, 16777472,
	15728880, 14680288, 13631696, 12583104,
	11534512, 10485920, 9437328, 8388736,
	7340144, 6291552, 5242960, 4194368,
	3145776, 2097184, 1048592, 0
};

static uint32_t igc_rgb[IGC_LUT_ENTRIES] = {
	4080, 4064, 4048, 4032, 4016, 4000, 3984, 3968, 3952, 3936, 3920, 3904,
	3888, 3872, 3856, 3840, 3824, 3808, 3792, 3776, 3760, 3744, 3728, 3712,
	3696, 3680, 3664, 3648, 3632, 3616, 3600, 3584, 3568, 3552, 3536, 3520,
	3504, 3488, 3472, 3456, 3440, 3424, 3408, 3392, 3376, 3360, 3344, 3328,
	3312, 3296, 3280, 3264, 3248, 3232, 3216, 3200, 3184, 3168, 3152, 3136,
	3120, 3104, 3088, 3072, 3056, 3040, 3024, 3008, 2992, 2976, 2960, 2944,
	2928, 2912, 2896, 2880, 2864, 2848, 2832, 2816, 2800, 2784, 2768, 2752,
	2736, 2720, 2704, 2688, 2672, 2656, 2640, 2624, 2608, 2592, 2576, 2560,
	2544, 2528, 2512, 2496, 2480, 2464, 2448, 2432, 2416, 2400, 2384, 2368,
	2352, 2336, 2320, 2304, 2288, 2272, 2256, 2240, 2224, 2208, 2192, 2176,
	2160, 2144, 2128, 2112, 2096, 2080, 2064, 2048, 2032, 2016, 2000, 1984,
	1968, 1952, 1936, 1920, 1904, 1888, 1872, 1856, 1840, 1824, 1808, 1792,
	1776, 1760, 1744, 1728, 1712, 1696, 1680, 1664, 1648, 1632, 1616, 1600,
	1584, 1568, 1552, 1536, 1520, 1504, 1488, 1472, 1456, 1440, 1424, 1408,
	1392, 1376, 1360, 1344, 1328, 1312, 1296, 1280, 1264, 1248, 1232, 1216,
	1200, 1184, 1168, 1152, 1136, 1120, 1104, 1088, 1072, 1056, 1040, 1024,
	1008, 992, 976, 960, 944, 928, 912, 896, 880, 864, 848, 832,
	816, 800, 784, 768, 752, 736, 720, 704, 688, 672, 656, 640,
	624, 608, 592, 576, 560, 544, 528, 512, 496, 480, 464, 448,
	432, 416, 400, 384, 368, 352, 336, 320, 304, 288, 272, 256,
	240, 224, 208, 192, 176, 160, 144, 128, 112, 96, 80, 64,
	48, 32, 16, 0
};

int write_igc(int invert)
{
	int fd;
	struct msmfb_mdp_pp pp;

	memset(&pp, 0, sizeof(struct msmfb_mdp_pp));

	pp.op = mdp_op_lut_cfg;
	pp.data.lut_cfg_data.lut_type = mdp_lut_igc;
	pp.data.lut_cfg_data.data.igc_lut_data.block = block;
	if (invert == 1)
		pp.data.lut_cfg_data.data.igc_lut_data.ops = ops_enable;
	else
		pp.data.lut_cfg_data.data.igc_lut_data.ops = ops_disable;
	pp.data.lut_cfg_data.data.igc_lut_data.len = IGC_LUT_ENTRIES;
	pp.data.lut_cfg_data.data.igc_lut_data.c0_c1_data = igc_inverted;
	pp.data.lut_cfg_data.data.igc_lut_data.c2_data = igc_rgb;

	write_pp(&pp);

	return 0;
}