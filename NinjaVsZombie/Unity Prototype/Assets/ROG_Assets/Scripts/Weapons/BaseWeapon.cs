using UnityEngine;
using System.Collections;

public class BaseWeapon : MonoBehaviour
{
	public 	float 		cooldown = 0;
	public 	float 		shotsPerSecond = 5;
	private float 		nextActivationTime = 0;
	
	protected virtual void  Start () 
	{
		
	}
	
	protected virtual void  Update () 
	{
	
	}
	
	public virtual bool CanActivate()
	{
		if(Time.time < nextActivationTime)
			return false;
		else			
			return true;
	}
	
	protected virtual void SetNewActivateTime()
	{
		if(shotsPerSecond <= 0)
			nextActivationTime = Time.time + cooldown;	
		else
			nextActivationTime = Time.time + Mathf.Max(1.0f/shotsPerSecond, cooldown);	
	}
	
	public virtual void Activate(Vector3 direction)
	{
		
	}
}
