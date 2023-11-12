# Decant

CAN-to-USB analyser. Designed to plug into existing CAN infrastructure for analysis. Focused on CANopen. UI is a work in progress, 

## Hardware

<div>
  <img src="https://github.com/HarveyBates/decant/blob/main/media/decant.png" width=300 align=right>
</div>

### Electronics
- [x] STM32F4 based processor
- [x] Two CAN interfaces with MCP2562 trancievers
- [x] USB powered
- [ ] Breakout other pins for various other functions
- [ ] Reduce footprint

#### BOM
See [BOM](electronics/bom/decant.csv), roughly $40 AUD ($25 USD). Main cost is the processor, however, this is the lowest cost STM32 that features 
CAN and USB on seperate pins.

#### PCB
See `electronics/decant/gerber` for [gerber files](https://github.com/HarveyBates/decant/tree/main/electronics/decant/gerber) 
that can be zipped and uploaded to a PCB manufactuer of choice. Get a top layer stencil too as this will make assembly easier.

## UI

This is a work in progress, currently using [Qt](https://www.qt.io/). Below is a TODO list to make it more functional.

<div>
  <img src="https://github.com/HarveyBates/decant/blob/main/media/ui_img.png" width=350 align=right>
</div>

### Base functions
- [x] Read raw CAN messages to table
- [ ] Filter messages
- [ ] Watch message
- [ ] Inspect bits
- [ ] Inspect message
- [ ] Graph selected bytes

### Database
- [ ] Connect DB
- [ ] Log raw to database
- [ ] Log message to DB (select bytes)

### Grafana 
- [ ] Graph from DB


