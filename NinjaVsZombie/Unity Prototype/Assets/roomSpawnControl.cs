using UnityEngine;
using System.Collections;

public class roomSpawnControl : MonoBehaviour 
{
	public ObjectSpawner spawner1;
	public ObjectSpawner spawner2;

	// Use this for initialization
	void Start () 
	{
		spawner1.spawnCap = true;
		spawner2.spawnCap = true;
		spawner1.spawnMax = 0;
		spawner2.spawnMax = 0;
	}
	
	// Update is called once per frame
	void Update () 
	{
		
	}

	void OnCollisionStay(Collision collisionData)
	{
		// Get the object that was hit
		GameObject other = collisionData.gameObject;

		if(other.tag == "Player")
		{
			spawner1.spawnCap = false;
			spawner2.spawnCap = false;

		}
	}

	void OnCollisionExit(Collision collisionData)
	{
		// Get the object that was hit
		GameObject other = collisionData.gameObject;

		if(other.tag == "Player")
		{
			spawner1.spawnCap = true;
			spawner2.spawnCap = true;
		}
	}
}
