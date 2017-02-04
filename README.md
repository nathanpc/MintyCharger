# MintyCharger

A very simple 9V (9.6V and 8.4V) NiMH charger based on a PIC microcontroller with a charge state indicator that fits neatly inside a Altoids tin.

## Schematic and Board Layout

A very simple topology, the PIC is generating a PWM pulse which is converted to a analog voltage by using a filter, which is fed into a op-amp that will be controlling the voltage. The current limit is set by the potentiometer `R10`. This is literally a miniature version of a primitive "lab" power supply.

![Schematic](http://i.imgur.com/kyl76EX.png)

As you can see the board is very tiny and is designed to fit inside a Altoids tin along side the battery that it will be charging.

![Board Layout](http://i.imgur.com/xK8btN8.png)
