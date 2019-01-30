# DeadlockBling
Our combat robot, [Deadlock](https://www.facebook.com/teamcyberwar/) has a light up panel that is remotely controlled. We use a spare channel from our radio recievers to change the pattern being displayed. The light up panel is made up from a microcontroller and sections of programmable LEDs.

<!-- picture of panel, working (in-situ?) -->

## Parts List

<!-- schematic/connections placeholder -->

Qty|Description|Link
---|-----------|----
1|Deadlock front armour|n/a
1|Trinket M0|[Adafruit](https://learn.adafruit.com/adafruit-trinket-m0-circuitpython-arduino)
2m|Neopixel 60LED/m Strip|[Adafruit](https://www.adafruit.com/product/2837?length=2)
1|48V to 5V|?

PSU notes:
* Cheap DCDC converters from eBay have terrible voltage trimmers
  * Replace with 2 resistors to set to +5V <!-- http://www.ti.com/download/kbase/volt/volt_div3.htm -->
  * Consider replacing counterfit LM2596HV

## Patterns

### Larson Scanner
### Breathing Effect
### Orange Terminal
### Rainbow Sparkles

## Licence
<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.
