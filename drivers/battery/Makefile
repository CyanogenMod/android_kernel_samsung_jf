obj-$(CONFIG_BATTERY_SAMSUNG)	+= sec_battery.o	\
								sec_fuelgauge.o

obj-$(CONFIG_FUELGAUGE_MAX17042)	+= max17042_fuelgauge.o
obj-$(CONFIG_FUELGAUGE_MAX17048)	+= max17048_fuelgauge.o
obj-$(CONFIG_FUELGAUGE_MAX17050)	+= max17050_fuelgauge.o

obj-$(CONFIG_CHARGER_MAX8903)	+= max8903_charger.o
obj-$(CONFIG_CHARGER_SMB328)	+= smb328_charger.o
obj-$(CONFIG_CHARGER_SMB347)	+= smb347_charger.o
obj-$(CONFIG_CHARGER_BQ24157)	+= bq24157_charger.o sec_charger.o
obj-$(CONFIG_CHARGER_BQ24190)	+= bq24190_charger.o
obj-$(CONFIG_CHARGER_BQ24191)	+= bq24190_charger.o
ifeq ($(CONFIG_MACH_MELIUS),y)
obj-$(CONFIG_CHARGER_MAX77XXX)	+= max77693_charger.o
else
obj-$(CONFIG_CHARGER_MAX77693)	+= max77693_charger.o
endif
