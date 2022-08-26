#include "Luna.h"
#include <imgui/imgui.h>

#include <random>

//Namespace for bubble sort algorithm
namespace BubbleSort {

	static bool add = false;
	static bool sort = false;
	static bool isAdding = false;

	static float amount = 10.0f;
	static int sortedAmount = 0;
	static float pillarWidth = 5.0f;


	struct Pillar
	{
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Luna::Anchor transform;
		int value = 0;
		bool hasBeenSorted = false;
	};

	std::vector<Pillar> pillars;


	bool GetStartState()
	{
		return add;
	}

	bool GetSortState()
	{
		return sort;
	}

	void ActivateSorting()
	{
		sort = true;
	}

	int GetAmountOfStartingElements()
	{
		return amount;
	}

	int GetSortedAmountOfPillars()
	{
		return sortedAmount;
	}
	int GetPillarAmount()
	{
		return pillars.size();
	}
	float GetPillarWidth()
	{
		return pillarWidth;
	}

	void Reset()
	{
		pillars.clear();
		sort = false;
		add = false;
		sortedAmount = 0;
	}

	void Add(float amount)
	{
		if (add)
		{
			isAdding = true;
			if (pillars.size() < amount)
			{
				for (int i = 0; i < 1; i++)
				{
					if (sort) break;

					Pillar pillar;
					int random = std::rand() % 1000 + 1; //Optionable?

					pillar.value = random;
					pillars.push_back(pillar);
				}
			}
		}
		isAdding = false;
	}

	//This is designed to be activated on a button press from imgui (or something similiar)
	void Sort(bool sort)
	{
		isAdding = false;
		if (sort && !isAdding && sortedAmount < pillars.size())
		{
			for (int i = 0; i < 1; i++)
			{
				for (int j = 0; j < pillars.size() - i - 1; j++)
				{
					float tempA = pillars[j].value;
					float tempB = pillars[j + 1].value;
					//float tempB = myRandomVector[static_cast<std::vector<Pillar, std::allocator<Pillar>>::size_type>(j) + 1].value;

					if (tempA > tempB)
					{
						pillars[j] = pillars[j + 1];
						pillars[j + 1].value = tempA;

						pillars[j].color = Luna::Colors::Red;
						pillars[j + 1].color = Luna::Colors::Red;
					}
				}
			}
			sortedAmount++;
			pillars[pillars.size() - sortedAmount].hasBeenSorted = true;
			pillars[pillars.size() - sortedAmount].color = { Luna::Colors::Green };
		}
	}

	//Add scale (X) as parameter
	void Render(float width) //GetPillarWidth(float xWidth)
	{
		/*
			x = 5, 260 in amount covers screen

		*/
		for (int i = 0; i < pillars.size(); i++)
		{
			pillars[i].transform.Scale.y = pillars[i].value;
			pillars[i].transform.Scale.x = width; //Works with 260x in amount

			pillars[i].transform.Translation.y = 0;
			pillars[i].transform.Translation.x = i * pillars[i].transform.Scale.x;

			Luna::Renderer::Draw(pillars[i].color, pillars[i].transform.GetTransform());
		}
	}
}

void Luna::Application::BuildUI()
{
	ImGui::Begin("Bubblesort Settings");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderFloat("Amount of pillars", &BubbleSort::amount, 0, 10000);
		ImGui::SliderFloat("Pillar width", &BubbleSort::pillarWidth, 0, 10);

		ImGui::Text("Amount of pillars in scene %d", BubbleSort::GetPillarAmount());
		ImGui::Text("Sorted pillars %d", BubbleSort::GetSortedAmountOfPillars());

		if (ImGui::Button("Start adding pillars"))
		{
			BubbleSort::add = true;
		}

		if (ImGui::Button("Sort pillars"))
		{
			BubbleSort::sort = true;
			BubbleSort::ActivateSorting();
		}
		if (ImGui::Button("Reset"))
		{
			BubbleSort::Reset();
		}

	ImGui::End();
}

int main()
{
	Luna::Application app("App");

	float lastFrameTime = 0.0f;


	while (app.IsRunning())
	{
		float elapsedTime = app.GetElapsedRuntime();
		Luna::DeltaTime deltaTime = elapsedTime - lastFrameTime;
		lastFrameTime = elapsedTime;

		app.Clear(0.0f, 0.0f, 0.0f, 1.0f);

		{ 
			// Amount of pillars (int)
			BubbleSort::Add(BubbleSort::GetAmountOfStartingElements());

			//Check if sorting can begin
			BubbleSort::Sort(BubbleSort::GetSortState());

			//Renders
			BubbleSort::Render(BubbleSort::GetPillarWidth());
		}

		app.Display();
	}
}