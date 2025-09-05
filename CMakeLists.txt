zephyr_library()

# このドライバを Kconfig 有効時のみビルド
zephyr_library_sources_ifdef(CONFIG_TDN_BATTERY_NIMH_DIVIDER
  drivers/sensor/battery/battery_nimh_divider.c
)
