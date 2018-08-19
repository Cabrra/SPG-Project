using UnityEngine;
using System.Collections;

public class CamControl : MonoBehaviour {
	public static CamControl Instance;
	public Vector3 camPos;
	public float moveSpeed = 10;
	// Use this for initialization
	void Start () 
	{
		Instance = this;
		camPos = new Vector3(0,20,0);
	}
	
	// Update is called once per frame
	void Update ()
	{
		float dist = Vector3.Distance(transform.position,camPos);
		Vector3 dir = (camPos - transform.position).normalized;

		if(dist > (moveSpeed*Time.deltaTime))
			transform.Translate(dir*moveSpeed*Time.deltaTime,Space.World);
		else
			transform.position = camPos;

		if (PauseScreen.Instance.miniMap == true)
			camera.enabled = false;
		else
			camera.enabled = true;
	}
}
