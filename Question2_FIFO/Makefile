default:
	rm myfifo
	mkfifo myfifo
	gcc Sender_FIFO.c -o Sender_FIFO.o
	gcc Reciever_FIFO.c -o Reciever_FIFO.o
	./Sender_FIFO.o & ./Reciever_FIFO.o
