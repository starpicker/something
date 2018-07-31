package com.example.slidejpeg;

import android.os.Bundle;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.view.Menu;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.ImageView;

@SuppressLint("SdCardPath")
public class MainActivity extends Activity {

	private static final String picPath = "/sdcard/lma/";
	private static final int    picNum = 15;
	private int current = -1;
	
	private static float DownX;
    private static ImageView iv;
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
		iv = (ImageView) this.findViewById(R.id.myview);
		
		
		showJPG(true);
		
		iv.setOnTouchListener(onTouch);
		
	}
	
	private void showJPG(boolean rightDirection)
	{
		if(rightDirection)
			current++;
		else
			current--;
		if(current <= 0)
			current = picNum - 1;
		current %= picNum;
		
		String fileName = picPath + current +".jpg";
		transfer(fileName);
		Bitmap bm = BitmapFactory.decodeFile(fileName);
		iv.setImageBitmap(bm);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
    
	 public OnTouchListener onTouch = new OnTouchListener(){
		    
	        @Override
	        public boolean onTouch(View v, MotionEvent event) {
	           
	        	
	            switch(event.getAction()){
	                case MotionEvent.ACTION_DOWN:
	        		    DownX = event.getX();
	                    break;
	                case MotionEvent.ACTION_UP:
	        		    if((event.getX() - DownX) >= (iv.getWidth()/4))
	        		    {
	        		    	//Log.e("slidejpeg", "sliding jpeg >>>>>>>>>>>>");
	        		    	showJPG(true);
	        		    }
	        		    else if(Math.abs((event.getX() - DownX)) >= (iv.getWidth()/4))
	        		    {
	        		    	//Log.e("slidejpeg", "sliding jpeg <<<<<<<<<<<<<");
	        		    	showJPG(false);
	        		    }
	                    break;
	            }
	            return true;
	        }
	    };

	    static {    
	         System.loadLibrary("transfer");    
	     }
	    private native final void transfer(String jpegPath); 
}
