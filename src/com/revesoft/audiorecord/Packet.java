package com.revesoft.audiorecord;

public class Packet {

	char[] data;
	int size;

	/*
	 * int stream_index; int flags; int side_data_elems; int duration; int pos;
	 * int convergence_duration;
	 */

	// public void init(int[]d,int s,int si,int fl,int sde,int dr,int p, int cd)
	public Packet(int size, char[] samples) {
		data = samples;
		this.size = size;
	}

	public void init(char[] d, int s) {
		data = d;
		size = s;
		/*
		 * stream_index=si; flags=fl; side_data_elems=sde; duration=dr; pos=p;
		 * convergence_duration=cd;
		 */
	}
}
