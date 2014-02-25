ManagePowerPC
=============

This tool is used to manage PowerPC where VxWorks is running. It is written in QT 4.X.

It is consisted of server and client. The server is like a proxy which is used to connect and occupy all idle PowerPC. The client can apply and give back PowerPC to server.

This tool is just like the computer-manage-system used in internet cafe. The server can manage computers automatically and the client is used by customer to apply computer to server.

---
Functions it supports:
- The PowerPC being managed can be configured in server's ppc.cfg file.
- Once the PowerPC being managed become idle, server will occupy it.
- The server can display all PowerPC's occupation status and client can get these information through update key.
- The client can apply PowerPC to server. And after finished using the PowerPC, client can give it back to server.

---
This tool is completed by using qttelnet-2.1_1-opensource and threaded server framework written by Francesc Martínez. Thank both of the writer.