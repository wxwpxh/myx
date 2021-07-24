mymotif@mymotif-ThinkPad-X201:~/prg/myx/XT/first$ ./1
Warning: XmPrimitive ClassInitialize: XmeTraitSet failed

Error: attempt to add non-widget child "DropSiteManager" to parent "1" which supports only widgets

解决：Motif FAQ (Part 9 of 9) Section - 321) Why do I get "Error: attempt to add non-widget child "dsm" to parent"?
http://www.faqs.org/faqs/motif-faq/part9/section-29.html

[Last modified: May 95]

Answer: Most likely, you are linking your libraries in the wrong order.  You
must link -lXm *before* -lXt.

Ken Lee

Ken Sall (ksall@cen.com) adds: This same error occurs if you combine Motif and
Athena widgets in the same application. If you link with "-lXaw" before "-
lXm", you get the runtime error. However, if you switch the order of the two
libraries, there is no problem. For example:

cc mothena.c -o mothena -lXm -lXaw -lXt -lXmu -lX11

