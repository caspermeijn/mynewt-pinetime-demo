<!--
#
#   Copyright 2020 Casper Meijn <casper@meijn.net>
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
-->

# mynewt demo for PineTime

## Overview

This is a demonstration application for the board support package of the Pine64 PineTime smartwatch. It uses all the functionalities used in the mynewt [bsp/pinetime](https://github.com/caspermeijn/mynewt-core).

## Status

Available drivers:

- Backlight blinking

## Usage

This repository contains some targets to test the board support. These can be build and run on the PineTime device.

It should also be possible to include this repo into other projects and just use the bsp part.

The bsp is configured to use openocd and a ST-LINK programmer. Connect your device to the ST-LINK and it to your computer. Then run the following commando to start the demo:

```bash
newt new mynewt-pinetime-demo
cd mynewt-pinetime-demo
newt sync

newt run blinky-pinetime 0
```

