ping -n 3 0.0.0.0>nul
@echo off
rd /s/q \\root\\anycubic\\project\\updater\\src\\AutoUpdater
del /s/q \root\anycubic\project\updater\src\AutoUpdater\del_self.bat