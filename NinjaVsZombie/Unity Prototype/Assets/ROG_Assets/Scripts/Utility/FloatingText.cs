using UnityEngine;
using System.Collections;

public class FloatingText
{
	public FloatingText(Vector3 worldPos, string text, Color color)
	{
		GameObject go = new GameObject("Floating Text");
		FloatingTextComponent floating = go.AddComponent<FloatingTextComponent>();
		floating.Begin(worldPos, 3.0f, -256, text, color);
	}
		
	public FloatingText(Vector3 worldPos, int yDistance, string text, Color color)
	{
		GameObject go = new GameObject("Floating Text");
		FloatingTextComponent floating = go.AddComponent<FloatingTextComponent>();
		floating.Begin(worldPos, 3.0f, yDistance, text, color);
	}
	
	public FloatingText(Vector3 worldPos, float duration, string text, Color color)
	{
		GameObject go = new GameObject("Floating Text");
		FloatingTextComponent floating = go.AddComponent<FloatingTextComponent>();
		floating.Begin(worldPos, duration, -256, text, color);
	}	
	
	public FloatingText(Vector3 worldPos, float duration, int yDistance, string text, Color color)
	{
		GameObject go = new GameObject("Floating Text");
		FloatingTextComponent floating = go.AddComponent<FloatingTextComponent>();
		floating.Begin(worldPos, duration, yDistance, text, color);
	}
}
