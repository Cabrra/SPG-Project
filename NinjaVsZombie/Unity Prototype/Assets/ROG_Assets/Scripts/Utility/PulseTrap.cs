using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class PulseTrap : MonoBehaviour 
{
	public 	float	damageAmount 	= 5.0f; // set or adjust in editor
	public	float	pulseRate		= 1.0f; // per second	
	private float	nextPulse		= 0;
	
	private	List<GameObject>	affectedList;
	
	void Start()
	{
		affectedList = new List<GameObject>();	
	}
	
	void Update()
	{
		if(Time.time > nextPulse)
		{
			for(int i = 0; i < affectedList.Count; i++)
			{
				if(affectedList[i] == null)			
					affectedList.RemoveAt(i);
				else
					affectedList[i].SendMessage("ModifyHealth", -damageAmount, SendMessageOptions.DontRequireReceiver);	
			}
			nextPulse = Time.time + pulseRate;
		}
	}
	
	void OnTriggerEnter(Collider other)
	{
		//print (other.name + " entered " + this.name);
		
		affectedList.Add(other.gameObject);		
	}
	
	void OnTriggerExit(Collider other)
	{
		//print (other.name + " exited " + this.name);
		
		int index = affectedList.FindIndex((x) => x.GetInstanceID() == other.gameObject.GetInstanceID());
	
		if(index != -1)
			affectedList.RemoveAt(index);
	}
}
