using UnityEngine;
using System.Collections;

public class CountdownTimer : MonoBehaviour {
	public float timer; // set duration time in seconds
	public float maxTime;
	public GUIStyle timerStyle;

	public AudioClip tictac;
	public bool tictacIsPlaying = false;
	public AudioClip TimeOver;
	public bool TimeisPlaying = false;

	// Use this for initialization
	void Start () {
		timer = 420;
		maxTime = 420;
	}
	
	// Update is called once per frame
	void Update ()
	{
		if (timer > 0)
			timer -= Time.deltaTime;
		if (timer <= 25) 
		{
			if(tictac && tictacIsPlaying == false)
				ROG.PlaySound(tictac);
			tictacIsPlaying = true;
		}
		if (timer <= 0) 
		{
			if(tictac && TimeisPlaying == false)
				ROG.PlaySound(TimeOver);
			TimeisPlaying = true;
		}
	}

	void OnGUI() 
	{
		float minutes = Mathf.Floor(timer / 60);
		float seconds = timer%60;

		//in seconds and milliseconds
		//ROG.DrawBar (timer / 300, 20, 20, 1000, 30, Color.green, timer.ToString("f2"));

		//In minutes and seconds
		if (timer >= (maxTime*2 / 3))
			ROG.DrawBar (timer / maxTime, 25, 20, 250, 30, Color.green,  minutes + ":" + Mathf.RoundToInt(seconds), timerStyle);
		else if (timer < (maxTime*2 / 3) && timer > (maxTime/3))
			ROG.DrawBar (timer / maxTime, 25, 20, 250, 30, Color.yellow,  minutes + ":" + Mathf.RoundToInt(seconds), timerStyle);
		else if (timer > 0)
			ROG.DrawBar (timer / maxTime, 25, 20, 250, 30, Color.red,  minutes + ":" + Mathf.RoundToInt(seconds), timerStyle);
		else
			ROG.DrawBar (timer / maxTime, 25, 20, 250, 30, Color.red,  "No extra tip", timerStyle);
		
	}
}