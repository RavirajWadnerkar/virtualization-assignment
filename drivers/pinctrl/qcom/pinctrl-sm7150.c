// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2018, The Linux Foundation. All rights reserved.
 * Copyright (c) 2023, Danila Tikhonov <danila@jiaxyga.com>
 */

#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pinctrl/pinctrl.h>

#include "pinctrl-msm.h"

static const char * const sm7150_tiles[] = {
	"north",
	"south",
	"west",
};

enum {
	NORTH,
	SOUTH,
	WEST
};

#define REG_SIZE 0x1000

#define PINGROUP(id, _tile, f1, f2, f3, f4, f5, f6, f7, f8, f9) \
	{						\
		.grp = PINCTRL_PINGROUP("gpio" #id, 	\
			gpio##id##_pins, 		\
			ARRAY_SIZE(gpio##id##_pins)),	\
		.funcs = (int[]){			\
			msm_mux_gpio, /* gpio mode */	\
			msm_mux_##f1,			\
			msm_mux_##f2,			\
			msm_mux_##f3,			\
			msm_mux_##f4,			\
			msm_mux_##f5,			\
			msm_mux_##f6,			\
			msm_mux_##f7,			\
			msm_mux_##f8,			\
			msm_mux_##f9			\
		},					\
		.nfuncs = 10,				\
		.ctl_reg = REG_SIZE * id,		\
		.io_reg = 0x4 + REG_SIZE * id,		\
		.intr_cfg_reg = 0x8 + REG_SIZE * id,	\
		.intr_status_reg = 0xc + REG_SIZE * id,	\
		.intr_target_reg = 0x8 + REG_SIZE * id,	\
		.tile = _tile,				\
		.mux_bit = 2,				\
		.pull_bit = 0,				\
		.drv_bit = 6,				\
		.oe_bit = 9,				\
		.in_bit = 0,				\
		.out_bit = 1,				\
		.intr_enable_bit = 0,			\
		.intr_status_bit = 0,			\
		.intr_target_bit = 5,			\
		.intr_target_kpss_val = 3,		\
		.intr_raw_status_bit = 4,		\
		.intr_polarity_bit = 1,			\
		.intr_detection_bit = 2,		\
		.intr_detection_width = 2,		\
	}

#define SDC_QDSD_PINGROUP(pg_name, ctl, pull, drv)	\
	{						\
		.grp = PINCTRL_PINGROUP(#pg_name, 	\
			pg_name##_pins, 		\
			ARRAY_SIZE(pg_name##_pins)),	\
		.ctl_reg = ctl,				\
		.io_reg = 0,				\
		.intr_cfg_reg = 0,			\
		.intr_status_reg = 0,			\
		.intr_target_reg = 0,			\
		.tile = SOUTH,				\
		.mux_bit = -1,				\
		.pull_bit = pull,			\
		.drv_bit = drv,				\
		.oe_bit = -1,				\
		.in_bit = -1,				\
		.out_bit = -1,				\
		.intr_enable_bit = -1,			\
		.intr_status_bit = -1,			\
		.intr_target_bit = -1,			\
		.intr_raw_status_bit = -1,		\
		.intr_polarity_bit = -1,		\
		.intr_detection_bit = -1,		\
		.intr_detection_width = -1,		\
	}

#define UFS_RESET(pg_name, offset)			\
	{						\
		.grp = PINCTRL_PINGROUP(#pg_name, 	\
			pg_name##_pins, 		\
			ARRAY_SIZE(pg_name##_pins)),	\
		.ctl_reg = offset,			\
		.io_reg = offset + 0x4,			\
		.intr_cfg_reg = 0,			\
		.intr_status_reg = 0,			\
		.intr_target_reg = 0,			\
		.tile = SOUTH,				\
		.mux_bit = -1,				\
		.pull_bit = 3,				\
		.drv_bit = 0,				\
		.oe_bit = -1,				\
		.in_bit = -1,				\
		.out_bit = 0,				\
		.intr_enable_bit = -1,			\
		.intr_status_bit = -1,			\
		.intr_target_bit = -1,			\
		.intr_raw_status_bit = -1,		\
		.intr_polarity_bit = -1,		\
		.intr_detection_bit = -1,		\
		.intr_detection_width = -1,		\
	}

static const struct pinctrl_pin_desc sm7150_pins[] = {
	PINCTRL_PIN(0, "GPIO_0"),
	PINCTRL_PIN(1, "GPIO_1"),
	PINCTRL_PIN(2, "GPIO_2"),
	PINCTRL_PIN(3, "GPIO_3"),
	PINCTRL_PIN(4, "GPIO_4"),
	PINCTRL_PIN(5, "GPIO_5"),
	PINCTRL_PIN(6, "GPIO_6"),
	PINCTRL_PIN(7, "GPIO_7"),
	PINCTRL_PIN(8, "GPIO_8"),
	PINCTRL_PIN(9, "GPIO_9"),
	PINCTRL_PIN(10, "GPIO_10"),
	PINCTRL_PIN(11, "GPIO_11"),
	PINCTRL_PIN(12, "GPIO_12"),
	PINCTRL_PIN(13, "GPIO_13"),
	PINCTRL_PIN(14, "GPIO_14"),
	PINCTRL_PIN(15, "GPIO_15"),
	PINCTRL_PIN(16, "GPIO_16"),
	PINCTRL_PIN(17, "GPIO_17"),
	PINCTRL_PIN(18, "GPIO_18"),
	PINCTRL_PIN(19, "GPIO_19"),
	PINCTRL_PIN(20, "GPIO_20"),
	PINCTRL_PIN(21, "GPIO_21"),
	PINCTRL_PIN(22, "GPIO_22"),
	PINCTRL_PIN(23, "GPIO_23"),
	PINCTRL_PIN(24, "GPIO_24"),
	PINCTRL_PIN(25, "GPIO_25"),
	PINCTRL_PIN(26, "GPIO_26"),
	PINCTRL_PIN(27, "GPIO_27"),
	PINCTRL_PIN(28, "GPIO_28"),
	PINCTRL_PIN(29, "GPIO_29"),
	PINCTRL_PIN(30, "GPIO_30"),
	PINCTRL_PIN(31, "GPIO_31"),
	PINCTRL_PIN(32, "GPIO_32"),
	PINCTRL_PIN(33, "GPIO_33"),
	PINCTRL_PIN(34, "GPIO_34"),
	PINCTRL_PIN(35, "GPIO_35"),
	PINCTRL_PIN(36, "GPIO_36"),
	PINCTRL_PIN(37, "GPIO_37"),
	PINCTRL_PIN(38, "GPIO_38"),
	PINCTRL_PIN(39, "GPIO_39"),
	PINCTRL_PIN(40, "GPIO_40"),
	PINCTRL_PIN(41, "GPIO_41"),
	PINCTRL_PIN(42, "GPIO_42"),
	PINCTRL_PIN(43, "GPIO_43"),
	PINCTRL_PIN(44, "GPIO_44"),
	PINCTRL_PIN(45, "GPIO_45"),
	PINCTRL_PIN(46, "GPIO_46"),
	PINCTRL_PIN(47, "GPIO_47"),
	PINCTRL_PIN(48, "GPIO_48"),
	PINCTRL_PIN(49, "GPIO_49"),
	PINCTRL_PIN(50, "GPIO_50"),
	PINCTRL_PIN(51, "GPIO_51"),
	PINCTRL_PIN(52, "GPIO_52"),
	PINCTRL_PIN(53, "GPIO_53"),
	PINCTRL_PIN(54, "GPIO_54"),
	PINCTRL_PIN(55, "GPIO_55"),
	PINCTRL_PIN(56, "GPIO_56"),
	PINCTRL_PIN(57, "GPIO_57"),
	PINCTRL_PIN(58, "GPIO_58"),
	PINCTRL_PIN(59, "GPIO_59"),
	PINCTRL_PIN(60, "GPIO_60"),
	PINCTRL_PIN(61, "GPIO_61"),
	PINCTRL_PIN(62, "GPIO_62"),
	PINCTRL_PIN(63, "GPIO_63"),
	PINCTRL_PIN(64, "GPIO_64"),
	PINCTRL_PIN(65, "GPIO_65"),
	PINCTRL_PIN(66, "GPIO_66"),
	PINCTRL_PIN(67, "GPIO_67"),
	PINCTRL_PIN(68, "GPIO_68"),
	PINCTRL_PIN(69, "GPIO_69"),
	PINCTRL_PIN(70, "GPIO_70"),
	PINCTRL_PIN(71, "GPIO_71"),
	PINCTRL_PIN(72, "GPIO_72"),
	PINCTRL_PIN(73, "GPIO_73"),
	PINCTRL_PIN(74, "GPIO_74"),
	PINCTRL_PIN(75, "GPIO_75"),
	PINCTRL_PIN(76, "GPIO_76"),
	PINCTRL_PIN(77, "GPIO_77"),
	PINCTRL_PIN(78, "GPIO_78"),
	PINCTRL_PIN(79, "GPIO_79"),
	PINCTRL_PIN(80, "GPIO_80"),
	PINCTRL_PIN(81, "GPIO_81"),
	PINCTRL_PIN(82, "GPIO_82"),
	PINCTRL_PIN(83, "GPIO_83"),
	PINCTRL_PIN(84, "GPIO_84"),
	PINCTRL_PIN(85, "GPIO_85"),
	PINCTRL_PIN(86, "GPIO_86"),
	PINCTRL_PIN(87, "GPIO_87"),
	PINCTRL_PIN(88, "GPIO_88"),
	PINCTRL_PIN(89, "GPIO_89"),
	PINCTRL_PIN(90, "GPIO_90"),
	PINCTRL_PIN(91, "GPIO_91"),
	PINCTRL_PIN(92, "GPIO_92"),
	PINCTRL_PIN(93, "GPIO_93"),
	PINCTRL_PIN(94, "GPIO_94"),
	PINCTRL_PIN(95, "GPIO_95"),
	PINCTRL_PIN(96, "GPIO_96"),
	PINCTRL_PIN(97, "GPIO_97"),
	PINCTRL_PIN(98, "GPIO_98"),
	PINCTRL_PIN(99, "GPIO_99"),
	PINCTRL_PIN(100, "GPIO_100"),
	PINCTRL_PIN(101, "GPIO_101"),
	PINCTRL_PIN(102, "GPIO_102"),
	PINCTRL_PIN(103, "GPIO_103"),
	PINCTRL_PIN(104, "GPIO_104"),
	PINCTRL_PIN(105, "GPIO_105"),
	PINCTRL_PIN(106, "GPIO_106"),
	PINCTRL_PIN(107, "GPIO_107"),
	PINCTRL_PIN(108, "GPIO_108"),
	PINCTRL_PIN(109, "GPIO_109"),
	PINCTRL_PIN(110, "GPIO_110"),
	PINCTRL_PIN(111, "GPIO_111"),
	PINCTRL_PIN(112, "GPIO_112"),
	PINCTRL_PIN(113, "GPIO_113"),
	PINCTRL_PIN(114, "GPIO_114"),
	PINCTRL_PIN(115, "GPIO_115"),
	PINCTRL_PIN(116, "GPIO_116"),
	PINCTRL_PIN(117, "GPIO_117"),
	PINCTRL_PIN(118, "GPIO_118"),
	PINCTRL_PIN(119, "UFS_RESET"),
	PINCTRL_PIN(120, "SDC1_RCLK"),
	PINCTRL_PIN(121, "SDC1_CLK"),
	PINCTRL_PIN(122, "SDC1_CMD"),
	PINCTRL_PIN(123, "SDC1_DATA"),
	PINCTRL_PIN(124, "SDC2_CLK"),
	PINCTRL_PIN(125, "SDC2_CMD"),
	PINCTRL_PIN(126, "SDC2_DATA"),

};

#define DECLARE_MSM_GPIO_PINS(pin) \
	static const unsigned int gpio##pin##_pins[] = { pin }
DECLARE_MSM_GPIO_PINS(0);
DECLARE_MSM_GPIO_PINS(1);
DECLARE_MSM_GPIO_PINS(2);
DECLARE_MSM_GPIO_PINS(3);
DECLARE_MSM_GPIO_PINS(4);
DECLARE_MSM_GPIO_PINS(5);
DECLARE_MSM_GPIO_PINS(6);
DECLARE_MSM_GPIO_PINS(7);
DECLARE_MSM_GPIO_PINS(8);
DECLARE_MSM_GPIO_PINS(9);
DECLARE_MSM_GPIO_PINS(10);
DECLARE_MSM_GPIO_PINS(11);
DECLARE_MSM_GPIO_PINS(12);
DECLARE_MSM_GPIO_PINS(13);
DECLARE_MSM_GPIO_PINS(14);
DECLARE_MSM_GPIO_PINS(15);
DECLARE_MSM_GPIO_PINS(16);
DECLARE_MSM_GPIO_PINS(17);
DECLARE_MSM_GPIO_PINS(18);
DECLARE_MSM_GPIO_PINS(19);
DECLARE_MSM_GPIO_PINS(20);
DECLARE_MSM_GPIO_PINS(21);
DECLARE_MSM_GPIO_PINS(22);
DECLARE_MSM_GPIO_PINS(23);
DECLARE_MSM_GPIO_PINS(24);
DECLARE_MSM_GPIO_PINS(25);
DECLARE_MSM_GPIO_PINS(26);
DECLARE_MSM_GPIO_PINS(27);
DECLARE_MSM_GPIO_PINS(28);
DECLARE_MSM_GPIO_PINS(29);
DECLARE_MSM_GPIO_PINS(30);
DECLARE_MSM_GPIO_PINS(31);
DECLARE_MSM_GPIO_PINS(32);
DECLARE_MSM_GPIO_PINS(33);
DECLARE_MSM_GPIO_PINS(34);
DECLARE_MSM_GPIO_PINS(35);
DECLARE_MSM_GPIO_PINS(36);
DECLARE_MSM_GPIO_PINS(37);
DECLARE_MSM_GPIO_PINS(38);
DECLARE_MSM_GPIO_PINS(39);
DECLARE_MSM_GPIO_PINS(40);
DECLARE_MSM_GPIO_PINS(41);
DECLARE_MSM_GPIO_PINS(42);
DECLARE_MSM_GPIO_PINS(43);
DECLARE_MSM_GPIO_PINS(44);
DECLARE_MSM_GPIO_PINS(45);
DECLARE_MSM_GPIO_PINS(46);
DECLARE_MSM_GPIO_PINS(47);
DECLARE_MSM_GPIO_PINS(48);
DECLARE_MSM_GPIO_PINS(49);
DECLARE_MSM_GPIO_PINS(50);
DECLARE_MSM_GPIO_PINS(51);
DECLARE_MSM_GPIO_PINS(52);
DECLARE_MSM_GPIO_PINS(53);
DECLARE_MSM_GPIO_PINS(54);
DECLARE_MSM_GPIO_PINS(55);
DECLARE_MSM_GPIO_PINS(56);
DECLARE_MSM_GPIO_PINS(57);
DECLARE_MSM_GPIO_PINS(58);
DECLARE_MSM_GPIO_PINS(59);
DECLARE_MSM_GPIO_PINS(60);
DECLARE_MSM_GPIO_PINS(61);
DECLARE_MSM_GPIO_PINS(62);
DECLARE_MSM_GPIO_PINS(63);
DECLARE_MSM_GPIO_PINS(64);
DECLARE_MSM_GPIO_PINS(65);
DECLARE_MSM_GPIO_PINS(66);
DECLARE_MSM_GPIO_PINS(67);
DECLARE_MSM_GPIO_PINS(68);
DECLARE_MSM_GPIO_PINS(69);
DECLARE_MSM_GPIO_PINS(70);
DECLARE_MSM_GPIO_PINS(71);
DECLARE_MSM_GPIO_PINS(72);
DECLARE_MSM_GPIO_PINS(73);
DECLARE_MSM_GPIO_PINS(74);
DECLARE_MSM_GPIO_PINS(75);
DECLARE_MSM_GPIO_PINS(76);
DECLARE_MSM_GPIO_PINS(77);
DECLARE_MSM_GPIO_PINS(78);
DECLARE_MSM_GPIO_PINS(79);
DECLARE_MSM_GPIO_PINS(80);
DECLARE_MSM_GPIO_PINS(81);
DECLARE_MSM_GPIO_PINS(82);
DECLARE_MSM_GPIO_PINS(83);
DECLARE_MSM_GPIO_PINS(84);
DECLARE_MSM_GPIO_PINS(85);
DECLARE_MSM_GPIO_PINS(86);
DECLARE_MSM_GPIO_PINS(87);
DECLARE_MSM_GPIO_PINS(88);
DECLARE_MSM_GPIO_PINS(89);
DECLARE_MSM_GPIO_PINS(90);
DECLARE_MSM_GPIO_PINS(91);
DECLARE_MSM_GPIO_PINS(92);
DECLARE_MSM_GPIO_PINS(93);
DECLARE_MSM_GPIO_PINS(94);
DECLARE_MSM_GPIO_PINS(95);
DECLARE_MSM_GPIO_PINS(96);
DECLARE_MSM_GPIO_PINS(97);
DECLARE_MSM_GPIO_PINS(98);
DECLARE_MSM_GPIO_PINS(99);
DECLARE_MSM_GPIO_PINS(100);
DECLARE_MSM_GPIO_PINS(101);
DECLARE_MSM_GPIO_PINS(102);
DECLARE_MSM_GPIO_PINS(103);
DECLARE_MSM_GPIO_PINS(104);
DECLARE_MSM_GPIO_PINS(105);
DECLARE_MSM_GPIO_PINS(106);
DECLARE_MSM_GPIO_PINS(107);
DECLARE_MSM_GPIO_PINS(108);
DECLARE_MSM_GPIO_PINS(109);
DECLARE_MSM_GPIO_PINS(110);
DECLARE_MSM_GPIO_PINS(111);
DECLARE_MSM_GPIO_PINS(112);
DECLARE_MSM_GPIO_PINS(113);
DECLARE_MSM_GPIO_PINS(114);
DECLARE_MSM_GPIO_PINS(115);
DECLARE_MSM_GPIO_PINS(116);
DECLARE_MSM_GPIO_PINS(117);
DECLARE_MSM_GPIO_PINS(118);

static const unsigned int ufs_reset_pins[] = { 119 };
static const unsigned int sdc1_rclk_pins[] = { 120 };
static const unsigned int sdc1_clk_pins[] = { 121 };
static const unsigned int sdc1_cmd_pins[] = { 122 };
static const unsigned int sdc1_data_pins[] = { 123 };
static const unsigned int sdc2_clk_pins[] = { 124 };
static const unsigned int sdc2_cmd_pins[] = { 125 };
static const unsigned int sdc2_data_pins[] = { 126 };

enum sm7150_functions {
	msm_mux_gpio,
	msm_mux_adsp_ext,
	msm_mux_agera_pll,
	msm_mux_aoss_cti,
	msm_mux_atest_char,
	msm_mux_atest_tsens,
	msm_mux_atest_tsens2,
	msm_mux_atest_usb1,
	msm_mux_atest_usb2,
	msm_mux_cam_mclk,
	msm_mux_cci_async,
	msm_mux_cci_i2c,
	msm_mux_cci_timer0,
	msm_mux_cci_timer1,
	msm_mux_cci_timer2,
	msm_mux_cci_timer3,
	msm_mux_cci_timer4,
	msm_mux_dbg_out,
	msm_mux_ddr_bist,
	msm_mux_ddr_pxi0,
	msm_mux_ddr_pxi1,
	msm_mux_ddr_pxi2,
	msm_mux_ddr_pxi3,
	msm_mux_edp_hot,
	msm_mux_edp_lcd,
	msm_mux_gcc_gp1,
	msm_mux_gcc_gp2,
	msm_mux_gcc_gp3,
	msm_mux_gp_pdm0,
	msm_mux_gp_pdm1,
	msm_mux_gp_pdm2,
	msm_mux_gps_tx,
	msm_mux_jitter_bist,
	msm_mux_ldo_en,
	msm_mux_ldo_update,
	msm_mux_m_voc,
	msm_mux_mdp_vsync,
	msm_mux_mdp_vsync0,
	msm_mux_mdp_vsync1,
	msm_mux_mdp_vsync2,
	msm_mux_mdp_vsync3,
	msm_mux_mss_lte,
	msm_mux_nav_pps_in,
	msm_mux_nav_pps_out,
	msm_mux_pa_indicator,
	msm_mux_pci_e,
	msm_mux_phase_flag,
	msm_mux_pll_bist,
	msm_mux_pll_bypassnl,
	msm_mux_pll_reset,
	msm_mux_pri_mi2s,
	msm_mux_pri_mi2s_ws,
	msm_mux_prng_rosc,
	msm_mux_qdss,
	msm_mux_qdss_cti,
	msm_mux_qlink_enable,
	msm_mux_qlink_request,
	msm_mux_qua_mi2s,
	msm_mux_qup00,
	msm_mux_qup01,
	msm_mux_qup02,
	msm_mux_qup03,
	msm_mux_qup04,
	msm_mux_qup10,
	msm_mux_qup11,
	msm_mux_qup12,
	msm_mux_qup13,
	msm_mux_qup14,
	msm_mux_qup15,
	msm_mux_sd_write,
	msm_mux_sdc40,
	msm_mux_sdc41,
	msm_mux_sdc42,
	msm_mux_sdc43,
	msm_mux_sdc4_clk,
	msm_mux_sdc4_cmd,
	msm_mux_sec_mi2s,
	msm_mux_ter_mi2s,
	msm_mux_tgu_ch0,
	msm_mux_tgu_ch1,
	msm_mux_tgu_ch2,
	msm_mux_tgu_ch3,
	msm_mux_tsif1_clk,
	msm_mux_tsif1_data,
	msm_mux_tsif1_en,
	msm_mux_tsif1_error,
	msm_mux_tsif1_sync,
	msm_mux_tsif2_clk,
	msm_mux_tsif2_data,
	msm_mux_tsif2_en,
	msm_mux_tsif2_error,
	msm_mux_tsif2_sync,
	msm_mux_uim1_clk,
	msm_mux_uim1_data,
	msm_mux_uim1_present,
	msm_mux_uim1_reset,
	msm_mux_uim2_clk,
	msm_mux_uim2_data,
	msm_mux_uim2_present,
	msm_mux_uim2_reset,
	msm_mux_uim_batt,
	msm_mux_usb_phy,
	msm_mux_vfr_1,
	msm_mux_vsense_trigger,
	msm_mux_wlan1_adc0,
	msm_mux_wlan1_adc1,
	msm_mux_wlan2_adc0,
	msm_mux_wlan2_adc1,
	msm_mux_wsa_clk,
	msm_mux_wsa_data,
	msm_mux__,
};

static const char * const gpio_groups[] = {
	"gpio0", "gpio1", "gpio2", "gpio3", "gpio4", "gpio5", "gpio6", "gpio7",
	"gpio8", "gpio9", "gpio10", "gpio11", "gpio12", "gpio13", "gpio14",
	"gpio15", "gpio16", "gpio17", "gpio18", "gpio19", "gpio20", "gpio21",
	"gpio22", "gpio23", "gpio24", "gpio25", "gpio26", "gpio27", "gpio28",
	"gpio29", "gpio30", "gpio31", "gpio32", "gpio33", "gpio34", "gpio35",
	"gpio36", "gpio37", "gpio38", "gpio39", "gpio40", "gpio41", "gpio42",
	"gpio43", "gpio44", "gpio45", "gpio46", "gpio47", "gpio48", "gpio49",
	"gpio50", "gpio51", "gpio52", "gpio53", "gpio54", "gpio55", "gpio56",
	"gpio57", "gpio58", "gpio59", "gpio60", "gpio61", "gpio62", "gpio63",
	"gpio64", "gpio65", "gpio66", "gpio67", "gpio68", "gpio69", "gpio70",
	"gpio71", "gpio72", "gpio73", "gpio74", "gpio75", "gpio76", "gpio77",
	"gpio78", "gpio79", "gpio80", "gpio81", "gpio82", "gpio83", "gpio84",
	"gpio85", "gpio86", "gpio87", "gpio88", "gpio89", "gpio90", "gpio91",
	"gpio92", "gpio93", "gpio94", "gpio95", "gpio96", "gpio97", "gpio98",
	"gpio99", "gpio100", "gpio101", "gpio102", "gpio103", "gpio104",
	"gpio105", "gpio106", "gpio107", "gpio108", "gpio109", "gpio110",
	"gpio111", "gpio112", "gpio113", "gpio114", "gpio115", "gpio116",
	"gpio117", "gpio118",
};

static const char * const adsp_ext_groups[] = {
	"gpio87",
};

static const char * const agera_pll_groups[] = {
	"gpio28",
};

static const char * const aoss_cti_groups[] = {
	"gpio85",
};

static const char * const atest_char_groups[] = {
	"gpio86", "gpio87", "gpio88", "gpio89", "gpio90",
};

static const char * const atest_tsens_groups[] = {
	"gpio29",
};

static const char * const atest_tsens2_groups[] = {
	"gpio7",
};

static const char * const atest_usb1_groups[] = {
	"gpio7", "gpio10", "gpio11", "gpio39", "gpio44",
};

static const char * const atest_usb2_groups[] = {
	"gpio51", "gpio52", "gpio53", "gpio54", "gpio55"
};

static const char * const cam_mclk_groups[] = {
	"gpio13", "gpio14", "gpio15", "gpio16",
};

static const char * const cci_async_groups[] = {
	"gpio24", "gpio25", "gpio26",
};

static const char * const cci_i2c_groups[] = {
	"gpio17", "gpio18", "gpio19", "gpio20", "gpio27", "gpio28",
};

static const char * const cci_timer0_groups[] = {
	"gpio21",
};

static const char * const cci_timer1_groups[] = {
	"gpio22",
};

static const char * const cci_timer2_groups[] = {
	"gpio23",
};

static const char * const cci_timer3_groups[] = {
	"gpio24",
};

static const char * const cci_timer4_groups[] = {
	"gpio25",
};

static const char * const dbg_out_groups[] = {
	"gpio3",
};

static const char * const ddr_bist_groups[] = {
	"gpio7", "gpio8", "gpio9", "gpio10",
};

static const char * const ddr_pxi0_groups[] = {
	"gpio6", "gpio7",
};

static const char * const ddr_pxi1_groups[] = {
	"gpio39", "gpio44",
};

static const char * const ddr_pxi2_groups[] = {
	"gpio10", "gpio11",
};

static const char * const ddr_pxi3_groups[] = {
	"gpio12", "gpio13",
};

static const char * const edp_hot_groups[] = {
	"gpio85",
};

static const char * const edp_lcd_groups[] = {
	"gpio11",
};

static const char * const gcc_gp1_groups[] = {
	"gpio48", "gpio56",
};

static const char * const gcc_gp2_groups[] = {
	"gpio21",
};

static const char * const gcc_gp3_groups[] = {
	"gpio22",
};

static const char * const gp_pdm0_groups[] = {
	"gpio37", "gpio68",
};

static const char * const gp_pdm1_groups[] = {
	"gpio8", "gpio50",
};

static const char * const gp_pdm2_groups[] = {
	"gpio57",
};

static const char * const gps_tx_groups[] = {
	"gpio83", "gpio84", "gpio107", "gpio109",
};

static const char * const jitter_bist_groups[] = {
	"gpio26",
};

static const char * const ldo_en_groups[] = {
	"gpio70",
};

static const char * const ldo_update_groups[] = {
	"gpio71",
};

static const char * const m_voc_groups[] = {
	"gpio12",
};

static const char * const mdp_vsync_groups[] = {
	"gpio10", "gpio11", "gpio12", "gpio70", "gpio71",
};

static const char * const mdp_vsync0_groups[] = {
	"gpio63",
};

static const char * const mdp_vsync1_groups[] = {
	"gpio63",
};

static const char * const mdp_vsync2_groups[] = {
	"gpio63",
};

static const char * const mdp_vsync3_groups[] = {
	"gpio63",
};

static const char * const mss_lte_groups[] = {
	"gpio108", "gpio109",
};

static const char * const nav_pps_in_groups[] = {
	"gpio83", "gpio84", "gpio107",
};

static const char * const nav_pps_out_groups[] = {
	"gpio83", "gpio84", "gpio107",
};

static const char * const pa_indicator_groups[] = {
	"gpio99",
};

static const char * const pci_e_groups[] = {
	"gpio66", "gpio67", "gpio68",
};

static const char * const phase_flag_groups[] = {
	"gpio0", "gpio1", "gpio2", "gpio6", "gpio7", "gpio10", "gpio11",
	"gpio12", "gpio13", "gpio14", "gpio15", "gpio16", "gpio17", "gpio24",
	"gpio25", "gpio26", "gpio27", "gpio28", "gpio29", "gpio30", "gpio35",
	"gpio36", "gpio37", "gpio38", "gpio39", "gpio43", "gpio44", "gpio56",
	"gpio57", "gpio60", "gpio61", "gpio62",
};

static const char * const pll_bist_groups[] = {
	"gpio27",
};

static const char * const pll_bypassnl_groups[] = {
	"gpio13",
};

static const char * const pll_reset_groups[] = {
	"gpio14",
};

static const char * const pri_mi2s_groups[] = {
	"gpio49", "gpio51", "gpio52",
};

static const char * const pri_mi2s_ws_groups[] = {
	"gpio50",
};

static const char * const prng_rosc_groups[] = {
	"gpio72",
};

static const char * const qdss_groups[] = {
	"gpio13", "gpio86", "gpio14", "gpio87", "gpio15", "gpio88", "gpio16",
	"gpio89", "gpio17", "gpio90", "gpio18", "gpio91", "gpio19", "gpio34",
	"gpio20", "gpio35", "gpio21", "gpio53", "gpio22", "gpio30", "gpio23",
	"gpio54", "gpio24", "gpio55", "gpio25", "gpio57", "gpio26", "gpio31",
	"gpio27", "gpio56", "gpio28", "gpio36", "gpio29", "gpio37", "gpio93",
	"gpio104",
};

static const char * const qdss_cti_groups[] = {
	"gpio4", "gpio5", "gpio32", "gpio44", "gpio45", "gpio63",
};

static const char * const qlink_enable_groups[] = {
	"gpio97",
};

static const char * const qlink_request_groups[] = {
	"gpio96",
};

static const char * const qua_mi2s_groups[] = {
	"gpio58",
};

static const char * const qup00_groups[] = {
	"gpio49", "gpio50", "gpio51", "gpio52", "gpio57", "gpio58",
};

static const char * const qup01_groups[] = {
	"gpio0", "gpio1", "gpio2", "gpio3", "gpio12", "gpio37",
};

static const char * const qup02_groups[] = {
	"gpio34", "gpio35",
};

static const char * const qup03_groups[] = {
	"gpio38", "gpio39", "gpio40", "gpio41",
};

static const char * const qup04_groups[] = {
	"gpio53", "gpio54", "gpio55", "gpio56",
};

static const char * const qup10_groups[] = {
	"gpio59", "gpio60", "gpio61", "gpio62", "gpio63", "gpio64", "gpio65",
};

static const char * const qup11_groups[] = {
	"gpio6", "gpio7", "gpio8", "gpio9",
};

static const char * const qup12_groups[] = {
	"gpio42", "gpio43", "gpio44", "gpio45",
};

static const char * const qup13_groups[] = {
	"gpio46", "gpio47",
};

static const char * const qup14_groups[] = {
	"gpio110", "gpio111", "gpio112", "gpio113",
};

static const char * const qup15_groups[] = {
	"gpio92", "gpio101", "gpio102", "gpio103",
};

static const char * const sd_write_groups[] = {
	"gpio33",
};

static const char * const sdc40_groups[] = {
	"gpio69",
};

static const char * const sdc41_groups[] = {
	"gpio68",
};

static const char * const sdc42_groups[] = {
	"gpio67",
};

static const char * const sdc43_groups[] = {
	"gpio65",
};

static const char * const sdc4_clk_groups[] = {
	"gpio66",
};

static const char * const sdc4_cmd_groups[] = {
	"gpio64",
};

static const char * const sec_mi2s_groups[] = {
	"gpio57",
};

static const char * const ter_mi2s_groups[] = {
	"gpio53", "gpio54", "gpio55", "gpio56",
};

static const char * const tgu_ch0_groups[] = {
	"gpio63",
};

static const char * const tgu_ch1_groups[] = {
	"gpio64",
};

static const char * const tgu_ch2_groups[] = {
	"gpio65",
};

static const char * const tgu_ch3_groups[] = {
	"gpio62",
};

static const char * const tsif1_clk_groups[] = {
	"gpio62",
};

static const char * const tsif1_data_groups[] = {
	"gpio64",
};

static const char * const tsif1_en_groups[] = {
	"gpio63",
};

static const char * const tsif1_error_groups[] = {
	"gpio60",
};

static const char * const tsif1_sync_groups[] = {
	"gpio61",
};

static const char * const tsif2_clk_groups[] = {
	"gpio66",
};

static const char * const tsif2_data_groups[] = {
	"gpio68",
};

static const char * const tsif2_en_groups[] = {
	"gpio67",
};

static const char * const tsif2_error_groups[] = {
	"gpio65",
};

static const char * const tsif2_sync_groups[] = {
	"gpio69",
};

static const char * const uim1_clk_groups[] = {
	"gpio80",
};

static const char * const uim1_data_groups[] = {
	"gpio79",
};

static const char * const uim1_present_groups[] = {
	"gpio82",
};

static const char * const uim1_reset_groups[] = {
	"gpio81",
};

static const char * const uim2_clk_groups[] = {
	"gpio76",
};

static const char * const uim2_data_groups[] = {
	"gpio75",
};

static const char * const uim2_present_groups[] = {
	"gpio78",
};

static const char * const uim2_reset_groups[] = {
	"gpio77",
};

static const char * const uim_batt_groups[] = {
	"gpio85",
};

static const char * const usb_phy_groups[] = {
	"gpio104",
};

static const char * const vfr_1_groups[] = {
	"gpio65",
};

static const char * const vsense_trigger_groups[] = {
	"gpio7",
};

static const char * const wlan1_adc0_groups[] = {
	"gpio39",
};

static const char * const wlan1_adc1_groups[] = {
	"gpio44",
};

static const char * const wlan2_adc0_groups[] = {
	"gpio11",
};

static const char * const wlan2_adc1_groups[] = {
	"gpio10",
};

static const char * const wsa_clk_groups[] = {
	"gpio49",
};

static const char * const wsa_data_groups[] = {
	"gpio50",
};

static const struct pinfunction sm7150_functions[] = {
	MSM_PIN_FUNCTION(gpio),
	MSM_PIN_FUNCTION(adsp_ext),
	MSM_PIN_FUNCTION(agera_pll),
	MSM_PIN_FUNCTION(aoss_cti),
	MSM_PIN_FUNCTION(atest_char),
	MSM_PIN_FUNCTION(atest_tsens),
	MSM_PIN_FUNCTION(atest_tsens2),
	MSM_PIN_FUNCTION(atest_usb1),
	MSM_PIN_FUNCTION(atest_usb2),
	MSM_PIN_FUNCTION(cam_mclk),
	MSM_PIN_FUNCTION(cci_async),
	MSM_PIN_FUNCTION(cci_i2c),
	MSM_PIN_FUNCTION(cci_timer0),
	MSM_PIN_FUNCTION(cci_timer1),
	MSM_PIN_FUNCTION(cci_timer2),
	MSM_PIN_FUNCTION(cci_timer3),
	MSM_PIN_FUNCTION(cci_timer4),
	MSM_PIN_FUNCTION(dbg_out),
	MSM_PIN_FUNCTION(ddr_bist),
	MSM_PIN_FUNCTION(ddr_pxi0),
	MSM_PIN_FUNCTION(ddr_pxi1),
	MSM_PIN_FUNCTION(ddr_pxi2),
	MSM_PIN_FUNCTION(ddr_pxi3),
	MSM_PIN_FUNCTION(edp_hot),
	MSM_PIN_FUNCTION(edp_lcd),
	MSM_PIN_FUNCTION(gcc_gp1),
	MSM_PIN_FUNCTION(gcc_gp2),
	MSM_PIN_FUNCTION(gcc_gp3),
	MSM_PIN_FUNCTION(gp_pdm0),
	MSM_PIN_FUNCTION(gp_pdm1),
	MSM_PIN_FUNCTION(gp_pdm2),
	MSM_PIN_FUNCTION(gps_tx),
	MSM_PIN_FUNCTION(jitter_bist),
	MSM_PIN_FUNCTION(ldo_en),
	MSM_PIN_FUNCTION(ldo_update),
	MSM_PIN_FUNCTION(m_voc),
	MSM_PIN_FUNCTION(mdp_vsync),
	MSM_PIN_FUNCTION(mdp_vsync0),
	MSM_PIN_FUNCTION(mdp_vsync1),
	MSM_PIN_FUNCTION(mdp_vsync2),
	MSM_PIN_FUNCTION(mdp_vsync3),
	MSM_PIN_FUNCTION(mss_lte),
	MSM_PIN_FUNCTION(nav_pps_in),
	MSM_PIN_FUNCTION(nav_pps_out),
	MSM_PIN_FUNCTION(pa_indicator),
	MSM_PIN_FUNCTION(pci_e),
	MSM_PIN_FUNCTION(phase_flag),
	MSM_PIN_FUNCTION(pll_bist),
	MSM_PIN_FUNCTION(pll_bypassnl),
	MSM_PIN_FUNCTION(pll_reset),
	MSM_PIN_FUNCTION(pri_mi2s),
	MSM_PIN_FUNCTION(pri_mi2s_ws),
	MSM_PIN_FUNCTION(prng_rosc),
	MSM_PIN_FUNCTION(qdss_cti),
	MSM_PIN_FUNCTION(qdss),
	MSM_PIN_FUNCTION(qlink_enable),
	MSM_PIN_FUNCTION(qlink_request),
	MSM_PIN_FUNCTION(qua_mi2s),
	MSM_PIN_FUNCTION(qup00),
	MSM_PIN_FUNCTION(qup01),
	MSM_PIN_FUNCTION(qup02),
	MSM_PIN_FUNCTION(qup03),
	MSM_PIN_FUNCTION(qup04),
	MSM_PIN_FUNCTION(qup10),
	MSM_PIN_FUNCTION(qup11),
	MSM_PIN_FUNCTION(qup12),
	MSM_PIN_FUNCTION(qup13),
	MSM_PIN_FUNCTION(qup14),
	MSM_PIN_FUNCTION(qup15),
	MSM_PIN_FUNCTION(sd_write),
	MSM_PIN_FUNCTION(sdc40),
	MSM_PIN_FUNCTION(sdc41),
	MSM_PIN_FUNCTION(sdc42),
	MSM_PIN_FUNCTION(sdc43),
	MSM_PIN_FUNCTION(sdc4_clk),
	MSM_PIN_FUNCTION(sdc4_cmd),
	MSM_PIN_FUNCTION(sec_mi2s),
	MSM_PIN_FUNCTION(ter_mi2s),
	MSM_PIN_FUNCTION(tgu_ch0),
	MSM_PIN_FUNCTION(tgu_ch1),
	MSM_PIN_FUNCTION(tgu_ch2),
	MSM_PIN_FUNCTION(tgu_ch3),
	MSM_PIN_FUNCTION(tsif1_clk),
	MSM_PIN_FUNCTION(tsif1_data),
	MSM_PIN_FUNCTION(tsif1_en),
	MSM_PIN_FUNCTION(tsif1_error),
	MSM_PIN_FUNCTION(tsif1_sync),
	MSM_PIN_FUNCTION(tsif2_clk),
	MSM_PIN_FUNCTION(tsif2_data),
	MSM_PIN_FUNCTION(tsif2_en),
	MSM_PIN_FUNCTION(tsif2_error),
	MSM_PIN_FUNCTION(tsif2_sync),
	MSM_PIN_FUNCTION(uim1_clk),
	MSM_PIN_FUNCTION(uim1_data),
	MSM_PIN_FUNCTION(uim1_present),
	MSM_PIN_FUNCTION(uim1_reset),
	MSM_PIN_FUNCTION(uim2_clk),
	MSM_PIN_FUNCTION(uim2_data),
	MSM_PIN_FUNCTION(uim2_present),
	MSM_PIN_FUNCTION(uim2_reset),
	MSM_PIN_FUNCTION(uim_batt),
	MSM_PIN_FUNCTION(usb_phy),
	MSM_PIN_FUNCTION(vfr_1),
	MSM_PIN_FUNCTION(vsense_trigger),
	MSM_PIN_FUNCTION(wlan1_adc0),
	MSM_PIN_FUNCTION(wlan1_adc1),
	MSM_PIN_FUNCTION(wlan2_adc0),
	MSM_PIN_FUNCTION(wlan2_adc1),
	MSM_PIN_FUNCTION(wsa_clk),
	MSM_PIN_FUNCTION(wsa_data),
};

/*
 * Every pin is maintained as a single group, and missing or non-existing pin
 * would be maintained as dummy group to synchronize pin group index with
 * pin descriptor registered with pinctrl core.
 * Clients would not be able to request these dummy pin groups.
 */
static const struct msm_pingroup sm7150_groups[] = {
	[0] = PINGROUP(0, SOUTH, qup01, _, phase_flag, _, _, _, _, _, _),
	[1] = PINGROUP(1, SOUTH, qup01, _, phase_flag, _, _, _, _, _, _),
	[2] = PINGROUP(2, SOUTH, qup01, _, phase_flag, _, _, _, _, _, _),
	[3] = PINGROUP(3, SOUTH, qup01, dbg_out, _, _, _, _, _, _, _),
	[4] = PINGROUP(4, NORTH, _, qdss_cti, _, _, _, _, _, _, _),
	[5] = PINGROUP(5, NORTH, _, qdss_cti, _, _, _, _, _, _, _),
	[6] = PINGROUP(6, NORTH, qup11, _, phase_flag, ddr_pxi0, _, _, _, _, _),
	[7] = PINGROUP(7, NORTH, qup11, ddr_bist, _, phase_flag, atest_tsens2, vsense_trigger, atest_usb1, ddr_pxi0, _),
	[8] = PINGROUP(8, NORTH, qup11, gp_pdm1, ddr_bist, _, _, _, _, _, _),
	[9] = PINGROUP(9, NORTH, qup11, ddr_bist, _, _, _, _, _, _, _),
	[10] = PINGROUP(10, NORTH, mdp_vsync, ddr_bist, _, phase_flag, wlan2_adc1, atest_usb1, ddr_pxi2, _, _),
	[11] = PINGROUP(11, NORTH, mdp_vsync, edp_lcd, _, phase_flag, wlan2_adc0, atest_usb1, ddr_pxi2, _, _),
	[12] = PINGROUP(12, SOUTH, mdp_vsync, m_voc, qup01, _, phase_flag, ddr_pxi3, _, _, _),
	[13] = PINGROUP(13, SOUTH, cam_mclk, pll_bypassnl, _, phase_flag, qdss, ddr_pxi3, _, _, _),
	[14] = PINGROUP(14, SOUTH, cam_mclk, pll_reset, _, phase_flag, qdss, _, _, _, _),
	[15] = PINGROUP(15, SOUTH, cam_mclk, _, phase_flag, qdss, _, _, _, _, _),
	[16] = PINGROUP(16, SOUTH, cam_mclk, _, phase_flag, qdss, _, _, _, _, _),
	[17] = PINGROUP(17, SOUTH, cci_i2c, _, phase_flag, qdss, _, _, _, _, _),
	[18] = PINGROUP(18, SOUTH, cci_i2c, qdss, _, _, _, _, _, _, _),
	[19] = PINGROUP(19, SOUTH, cci_i2c, qdss, _, _, _, _, _, _, _),
	[20] = PINGROUP(20, SOUTH, cci_i2c, qdss, _, _, _, _, _, _, _),
	[21] = PINGROUP(21, SOUTH, cci_timer0, gcc_gp2, _, qdss, _, _, _, _, _),
	[22] = PINGROUP(22, SOUTH, cci_timer1, gcc_gp3, _, qdss, _, _, _, _, _),
	[23] = PINGROUP(23, SOUTH, cci_timer2, qdss, _, _, _, _, _, _, _),
	[24] = PINGROUP(24, SOUTH, cci_timer3, cci_async, _, phase_flag, qdss, _, _, _, _),
	[25] = PINGROUP(25, SOUTH, cci_timer4, cci_async, _, phase_flag, qdss, _, _, _, _),
	[26] = PINGROUP(26, SOUTH, cci_async, jitter_bist, _, phase_flag, qdss, _, _, _, _),
	[27] = PINGROUP(27, SOUTH, cci_i2c, pll_bist, _, phase_flag, qdss, _, _, _, _),
	[28] = PINGROUP(28, SOUTH, cci_i2c, agera_pll, _, phase_flag, qdss, _, _, _, _),
	[29] = PINGROUP(29, NORTH, _, _, phase_flag, qdss, atest_tsens, _, _, _, _),
	[30] = PINGROUP(30, SOUTH, _, phase_flag, qdss, _, _, _, _, _, _),
	[31] = PINGROUP(31, WEST, _, qdss, _, _, _, _, _, _, _),
	[32] = PINGROUP(32, NORTH, qdss_cti, _, _, _, _, _, _, _, _),
	[33] = PINGROUP(33, NORTH, sd_write, _, _, _, _, _, _, _, _),
	[34] = PINGROUP(34, SOUTH, qup02, qdss, _, _, _, _, _, _, _),
	[35] = PINGROUP(35, SOUTH, qup02, _, phase_flag, qdss, _, _, _, _, _),
	[36] = PINGROUP(36, SOUTH, _, phase_flag, qdss, _, _, _, _, _, _),
	[37] = PINGROUP(37, SOUTH, qup01, gp_pdm0, _, phase_flag, qdss, _, _, _, _),
	[38] = PINGROUP(38, SOUTH, qup03, _, phase_flag, _, _, _, _, _, _),
	[39] = PINGROUP(39, SOUTH, qup03, _, phase_flag, _, wlan1_adc0, atest_usb1, ddr_pxi1, _, _),
	[40] = PINGROUP(40, SOUTH, qup03, _, _, _, _, _, _, _, _),
	[41] = PINGROUP(41, SOUTH, qup03, _, _, _, _, _, _, _, _),
	[42] = PINGROUP(42, NORTH, qup12, _, _, _, _, _, _, _, _),
	[43] = PINGROUP(43, NORTH, qup12, _, phase_flag, _, _, _, _, _, _),
	[44] = PINGROUP(44, NORTH, qup12, _, phase_flag, qdss_cti, _, wlan1_adc1, atest_usb1, ddr_pxi1, _),
	[45] = PINGROUP(45, NORTH, qup12, qdss_cti, _, _, _, _, _, _, _),
	[46] = PINGROUP(46, NORTH, qup13, _, _, _, _, _, _, _, _),
	[47] = PINGROUP(47, NORTH, qup13, _, _, _, _, _, _, _, _),
	[48] = PINGROUP(48, WEST, gcc_gp1, _, _, _, _, _, _, _, _),
	[49] = PINGROUP(49, WEST, pri_mi2s, qup00, wsa_clk, _, _, _, _, _, _),
	[50] = PINGROUP(50, WEST, pri_mi2s_ws, qup00, wsa_data, gp_pdm1, _, _, _, _, _),
	[51] = PINGROUP(51, WEST, pri_mi2s, qup00, atest_usb2, _, _, _, _, _, _),
	[52] = PINGROUP(52, WEST, pri_mi2s, qup00, atest_usb2, _, _, _, _, _, _),
	[53] = PINGROUP(53, WEST, ter_mi2s, qup04, qdss, atest_usb2, _, _, _, _, _),
	[54] = PINGROUP(54, WEST, ter_mi2s, qup04, qdss, atest_usb2, _, _, _, _, _),
	[55] = PINGROUP(55, WEST, ter_mi2s, qup04, qdss, atest_usb2, _, _, _, _, _),
	[56] = PINGROUP(56, WEST, ter_mi2s, qup04, gcc_gp1, _, phase_flag, qdss, _, _, _),
	[57] = PINGROUP(57, WEST, sec_mi2s, qup00, gp_pdm2, _, phase_flag, qdss, _, _, _),
	[58] = PINGROUP(58, WEST, qua_mi2s, qup00, _, _, _, _, _, _, _),
	[59] = PINGROUP(59, NORTH, qup10, _, _, _, _, _, _, _, _),
	[60] = PINGROUP(60, NORTH, qup10, tsif1_error, _, phase_flag, _, _, _, _, _),
	[61] = PINGROUP(61, NORTH, qup10, tsif1_sync, _, phase_flag, _, _, _, _, _),
	[62] = PINGROUP(62, NORTH, qup10, tsif1_clk, tgu_ch3, _, phase_flag, _, _, _, _),
	[63] = PINGROUP(63, NORTH, tsif1_en, mdp_vsync0, qup10, mdp_vsync1, mdp_vsync2, mdp_vsync3, tgu_ch0, qdss_cti, _),
	[64] = PINGROUP(64, NORTH, tsif1_data, sdc4_cmd, qup10, tgu_ch1, _, _, _, _, _),
	[65] = PINGROUP(65, NORTH, tsif2_error, sdc43, qup10, vfr_1, tgu_ch2, _, _, _, _),
	[66] = PINGROUP(66, NORTH, tsif2_clk, sdc4_clk, pci_e, _, _, _, _, _, _),
	[67] = PINGROUP(67, NORTH, tsif2_en, sdc42, pci_e, _, _, _, _, _, _),
	[68] = PINGROUP(68, NORTH, tsif2_data, sdc41, pci_e, gp_pdm0, _, _, _, _, _),
	[69] = PINGROUP(69, NORTH, tsif2_sync, sdc40, _, _, _, _, _, _, _),
	[70] = PINGROUP(70, NORTH, _, _, mdp_vsync, ldo_en, _, _, _, _, _),
	[71] = PINGROUP(71, NORTH, _, mdp_vsync, ldo_update, _, _, _, _, _, _),
	[72] = PINGROUP(72, NORTH, prng_rosc, _, _, _, _, _, _, _, _),
	[73] = PINGROUP(73, NORTH, _, _, _, _, _, _, _, _, _),
	[74] = PINGROUP(74, WEST, _, _, _, _, _, _, _, _, _),
	[75] = PINGROUP(75, WEST, uim2_data, _, _, _, _, _, _, _, _),
	[76] = PINGROUP(76, WEST, uim2_clk, _, _, _, _, _, _, _, _),
	[77] = PINGROUP(77, WEST, uim2_reset, _, _, _, _, _, _, _, _),
	[78] = PINGROUP(78, WEST, uim2_present, _, _, _, _, _, _, _, _),
	[79] = PINGROUP(79, WEST, uim1_data, _, _, _, _, _, _, _, _),
	[80] = PINGROUP(80, WEST, uim1_clk, _, _, _, _, _, _, _, _),
	[81] = PINGROUP(81, WEST, uim1_reset, _, _, _, _, _, _, _, _),
	[82] = PINGROUP(82, WEST, uim1_present, _, _, _, _, _, _, _, _),
	[83] = PINGROUP(83, WEST, _, nav_pps_in, nav_pps_out, gps_tx, _, _, _, _, _),
	[84] = PINGROUP(84, WEST, _, nav_pps_in, nav_pps_out, gps_tx, _, _, _, _, _),
	[85] = PINGROUP(85, WEST, uim_batt, edp_hot, aoss_cti, _, _, _, _, _, _),
	[86] = PINGROUP(86, NORTH, qdss, atest_char, _, _, _, _, _, _, _),
	[87] = PINGROUP(87, NORTH, adsp_ext, qdss, atest_char, _, _, _, _, _, _),
	[88] = PINGROUP(88, NORTH, qdss, atest_char, _, _, _, _, _, _, _),
	[89] = PINGROUP(89, NORTH, qdss, atest_char, _, _, _, _, _, _, _),
	[90] = PINGROUP(90, NORTH, qdss, atest_char, _, _, _, _, _, _, _),
	[91] = PINGROUP(91, NORTH, qdss, _, _, _, _, _, _, _, _),
	[92] = PINGROUP(92, NORTH, _, _, qup15, _, _, _, _, _, _),
	[93] = PINGROUP(93, NORTH, qdss, _, _, _, _, _, _, _, _),
	[94] = PINGROUP(94, SOUTH, _, _, _, _, _, _, _, _, _),
	[95] = PINGROUP(95, WEST, _, _, _, _, _, _, _, _, _),
	[96] = PINGROUP(96, WEST, qlink_request, _, _, _, _, _, _, _, _),
	[97] = PINGROUP(97, WEST, qlink_enable, _, _, _, _, _, _, _, _),
	[98] = PINGROUP(98, WEST, _, _, _, _, _, _, _, _, _),
	[99] = PINGROUP(99, WEST, _, pa_indicator, _, _, _, _, _, _, _),
	[100] = PINGROUP(100, WEST, _, _, _, _, _, _, _, _, _),
	[101] = PINGROUP(101, NORTH, _, _, qup15, _, _, _, _, _, _),
	[102] = PINGROUP(102, NORTH, _, _, qup15, _, _, _, _, _, _),
	[103] = PINGROUP(103, NORTH, _, qup15, _, _, _, _, _, _, _),
	[104] = PINGROUP(104, WEST, usb_phy, _, qdss, _, _, _, _, _, _),
	[105] = PINGROUP(105, NORTH, _, _, _, _, _, _, _, _, _),
	[106] = PINGROUP(106, NORTH, _, _, _, _, _, _, _, _, _),
	[107] = PINGROUP(107, WEST, _, nav_pps_in, nav_pps_out, gps_tx, _, _, _, _, _),
	[108] = PINGROUP(108, SOUTH, mss_lte, _, _, _, _, _, _, _, _),
	[109] = PINGROUP(109, SOUTH, mss_lte, gps_tx, _, _, _, _, _, _, _),
	[110] = PINGROUP(110, NORTH, _, _, qup14, _, _, _, _, _, _),
	[111] = PINGROUP(111, NORTH, _, _, qup14, _, _, _, _, _, _),
	[112] = PINGROUP(112, NORTH, _, qup14, _, _, _, _, _, _, _),
	[113] = PINGROUP(113, NORTH, _, qup14, _, _, _, _, _, _, _),
	[114] = PINGROUP(114, NORTH, _, _, _, _, _, _, _, _, _),
	[115] = PINGROUP(115, NORTH, _, _, _, _, _, _, _, _, _),
	[116] = PINGROUP(116, NORTH, _, _, _, _, _, _, _, _, _),
	[117] = PINGROUP(117, NORTH, _, _, _, _, _, _, _, _, _),
	[118] = PINGROUP(118, NORTH, _, _, _, _, _, _, _, _, _),
	[119] = UFS_RESET(ufs_reset, 0x9f000),
	[120] = SDC_QDSD_PINGROUP(sdc1_rclk, 0x9a000, 15, 0),
	[121] = SDC_QDSD_PINGROUP(sdc1_clk, 0x9a000, 13, 6),
	[122] = SDC_QDSD_PINGROUP(sdc1_cmd, 0x9a000, 11, 3),
	[123] = SDC_QDSD_PINGROUP(sdc1_data, 0x9a000, 9, 0),
	[124] = SDC_QDSD_PINGROUP(sdc2_clk, 0x98000, 14, 6),
	[125] = SDC_QDSD_PINGROUP(sdc2_cmd, 0x98000, 11, 3),
	[126] = SDC_QDSD_PINGROUP(sdc2_data, 0x98000, 9, 0),
};

static const struct msm_gpio_wakeirq_map sm7150_pdc_map[] = {
	{0, 40}, {3, 50}, {4, 42}, {5, 70}, {6, 41}, {9, 57},
	{10, 80}, {11, 51}, {22, 90}, {24, 61}, {26, 52}, {30, 56},
	{31, 33}, {32, 81}, {33, 62}, {34, 43}, {36, 91}, {37, 53},
	{38, 63}, {39, 72}, {41, 101}, {42, 35}, {43, 34}, {45, 73},
	{47, 82}, {48, 36}, {49, 37}, {50, 38}, {52, 39}, {53, 102},
	{55, 92}, {56, 45}, {57, 46}, {58, 83}, {59, 47}, {62, 48},
	{64, 74}, {65, 44}, {66, 93}, {67, 49}, {68, 55}, {69, 32},
	{70, 54}, {73, 64}, {74, 71}, {78, 31}, {82, 30}, {84, 58},
	{85, 103}, {86, 59}, {87, 60}, {88, 65}, {89, 66}, {90, 67},
	{91, 68}, {92, 69}, {93, 75}, {94, 84}, {95, 94}, {96, 76},
	{98, 77}, {101, 78}, {104, 99}, {109, 104}, {110, 79}, {113, 85},
};

static const struct msm_pinctrl_soc_data sm7150_tlmm = {
	.pins = sm7150_pins,
	.npins = ARRAY_SIZE(sm7150_pins),
	.functions = sm7150_functions,
	.nfunctions = ARRAY_SIZE(sm7150_functions),
	.groups = sm7150_groups,
	.ngroups = ARRAY_SIZE(sm7150_groups),
	.ngpios = 120,
	.tiles = sm7150_tiles,
	.ntiles = ARRAY_SIZE(sm7150_tiles),
	.wakeirq_map = sm7150_pdc_map,
	.nwakeirq_map = ARRAY_SIZE(sm7150_pdc_map),
	.wakeirq_dual_edge_errata = true,
};

static int sm7150_tlmm_probe(struct platform_device *pdev)
{
	return msm_pinctrl_probe(pdev, &sm7150_tlmm);
}

static const struct of_device_id sm7150_tlmm_of_match[] = {
	{ .compatible = "qcom,sm7150-tlmm", },
	{ },
};

static struct platform_driver sm7150_tlmm_driver = {
	.driver = {
		.name = "sm7150-tlmm",
		.pm = &msm_pinctrl_dev_pm_ops,
		.of_match_table = sm7150_tlmm_of_match,
	},
	.probe = sm7150_tlmm_probe,
	.remove_new = msm_pinctrl_remove,
};

static int __init sm7150_tlmm_init(void)
{
	return platform_driver_register(&sm7150_tlmm_driver);
}
arch_initcall(sm7150_tlmm_init);

static void __exit sm7150_tlmm_exit(void)
{
	platform_driver_unregister(&sm7150_tlmm_driver);
}
module_exit(sm7150_tlmm_exit);

MODULE_DESCRIPTION("Qualcomm SM7150 TLMM driver");
MODULE_LICENSE("GPL");
