using UnityEngine;
using System.Collections;

public class PauseScreen : MonoBehaviour 
{
	public static PauseScreen Instance;
	public GUISkin myskin;
	
	private Rect windowRect;
	public bool isPaused = false;
	public bool miniMap = false;
	public bool xboxCont = false;
	public bool keyboardCont = false;
	
	private void Start()
	{
		Instance = this;
		windowRect = new Rect(Screen.width / 2 - 100, Screen.height/2 - 125, 200, 270);
		Time.timeScale = 1;

	}

	
	private void Update()
	{
		if (Input.GetKeyDown(KeyCode.P) || Input.GetKeyDown(KeyCode.JoystickButton7))
		{
			isPaused = !isPaused;
			if(isPaused == true)
				Time.timeScale = 0;
			else
				Time.timeScale = 1;
		}
		if (isPaused == true) 
		{
			if (Input.GetKeyDown (KeyCode.R) || Input.GetKeyDown(KeyCode.JoystickButton1))
				Application.LoadLevel (2);
			if (Input.GetKeyDown(KeyCode.M) || Input.GetKeyDown(KeyCode.JoystickButton6))
				Application.LoadLevel (1);
			if (Input.GetKeyDown(KeyCode.I) || Input.GetKeyDown(KeyCode.JoystickButton0))
				miniMap = !miniMap;
			if (Input.GetKeyDown(KeyCode.K))
				keyboardCont = !keyboardCont;
			if(Input.GetKeyDown(KeyCode.JoystickButton3))
				xboxCont = !xboxCont;
		}
		else
		{
			miniMap = false;
			xboxCont = false;
			keyboardCont = false;
		}
	}
	
	private void OnGUI()
	{
		if (isPaused == true && keyboardCont== false && xboxCont == false && miniMap == false)
			windowRect = GUI.Window(0, windowRect, windowFunc, "Pause Menu");
	}
	
	private void windowFunc(int id)
	{
			GUI.Button (new Rect (0, 20, 200, 50), "RESUME (P / Start)");
			GUI.Button (new Rect (0, 70, 200, 50), "RESTART (R / B)");
			GUI.Button (new Rect (0, 120, 200, 50), "CONTROLS (K / Y)");
			GUI.Button (new Rect (0, 170, 200, 50), "MINI NAP (I / A)");
			GUI.Button (new Rect (0, 220, 200, 50), "MAIN MENU (M / Select)");
	
	}
}
