package com.revesoft.audiorecord;

import android.util.Log;

public class OPUSInterface {
	static {
		System.loadLibrary("codec");
	}
	
	private static OPUSInterface opus = null;
	
	private OPUSInterface(){}
	
	public static synchronized OPUSInterface getInstance(int bufferSize){
		Log.d("debug", "::OPUSInterface getInstance");
		if(opus == null){
			opus = new OPUSInterface();
			opus.open(bufferSize);
		}
		return opus;
	}
	
	public native void open(int lenOfSamples);
	public native int encode(short[] samples, int lenOfSamples, char[] outputData);
	public native int decode(char[] inputData, int length, short[] outputSample);
	public native int close();

}
