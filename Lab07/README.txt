Hang Yuan, 1564348, hyuan3

This lab is basically wrap up all things we did in the previous lab together. It's important
to understand how finite state machine works in the program to implement event-driven program.
An important programming thought in this lab, I think, is the understanding of the logic of 
thr program design. It's hard to write the program without the design first. This lab is harder
then before.

I first read the entire lab instruction document and realise the basic OLED functions to show
the correct display including the "artwork". Then I implemennt the event triggers in ISP and
test them. With the event triggers, I started the state part which is the most difficult part
in this lab from the state SETUP to state RESET. The hardest part in these states is cooking,
where you have to handle multiple actions/events and the OLED update a lot. Finaly, I did the 
extra credit part.

The final program implementation looks good to me and every part in the lab have been implemented
well. At first my program had significant delay in swtiching states and selector. After a long
time debugging, I fixed this problem by clearing flags in FSM rather than main function which 
brings me good result.

For the EXTRA CREDIT, I added a new state named ALERT. When the remaining time in COOKING state
has 0 seconds, the state will be switched to ALERT where I used if-else to blink the screen.
When botton 4 is down, the state will be set back to SETUP and the screen stops blinking.
