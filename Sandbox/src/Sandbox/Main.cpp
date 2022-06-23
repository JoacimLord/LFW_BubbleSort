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

	glm::vec4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };

	struct Pillar
	{
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Luna::Anchor transform;
		int value = 0;
		bool hasBeenSorted = false;
	};

	std::vector<Pillar> myRandomVector;


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
		return myRandomVector.size();
	}
	float GetPillarWidth()
	{
		return pillarWidth;
	}

	void Reset()
	{
		myRandomVector.clear();
		sort = false;
		add = false;
		sortedAmount = 0;
	}

	void Add(float amount)
	{
		if (add)
		{
			isAdding = true;
			if (myRandomVector.size() < amount)
			{
				for (int i = 0; i < 1; i++)
				{
					if (sort) break;

					Pillar pillar;
					int random = std::rand() % 1000 + 1; //Optionable?

					pillar.value = random;
					myRandomVector.push_back(pillar);
				}
			}
		}
		isAdding = false;
	}

	//This is designed to be activated on a button press from imgui (or something similiar)
	void Sort(bool sort)
	{
		isAdding = false;
		if (sort && !isAdding && sortedAmount < myRandomVector.size())
		{
			for (int i = 0; i < 1; i++)
			{
				for (int j = 0; j < myRandomVector.size() - i - 1; j++)
				{
					float tempA = myRandomVector[j].value;
					float tempB = myRandomVector[j + 1].value;
					//float tempB = myRandomVector[static_cast<std::vector<Pillar, std::allocator<Pillar>>::size_type>(j) + 1].value;

					if (tempA > tempB)
					{
						myRandomVector[j] = myRandomVector[j + 1];
						myRandomVector[j + 1].value = tempA;

						myRandomVector[j].color = Red;
						myRandomVector[j + 1].color = Red;
					}
				}
			}
			sortedAmount++;
			myRandomVector[myRandomVector.size() - sortedAmount].hasBeenSorted = true;
			myRandomVector[myRandomVector.size() - sortedAmount].color = { Luna::Colors::Green };
		}
	}

	//Add scale (X) as parameter
	void Render(float width) //GetPillarWidth(float xWidth)
	{
		/*
			x = 5, 260 in amount covers screen

		*/
		for (int i = 0; i < myRandomVector.size(); i++)
		{
			myRandomVector[i].transform.Scale.y = myRandomVector[i].value;
			myRandomVector[i].transform.Scale.x = width; //Works with 260x in amount

			myRandomVector[i].transform.Translation.y = 0;
			myRandomVector[i].transform.Translation.x = i * myRandomVector[i].transform.Scale.x;

			Luna::Renderer::Draw(myRandomVector[i].color, myRandomVector[i].transform.GetTransform());
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

	//std::srand(time(0));

	float lastFrameTime = 0.0f;
	glm::vec4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };


	while (app.IsRunning())
	{
		float elapsedTime = app.GetElapsedRuntime();
		Luna::DeltaTime deltaTime = elapsedTime - lastFrameTime;
		lastFrameTime = elapsedTime;

		app.Clear(Black);

		//BubbleSort (Dataoriented approach)!
		{
			BubbleSort::Add(BubbleSort::GetAmountOfStartingElements()); // Takes amount (int)
			BubbleSort::Sort(BubbleSort::GetSortState()); //Check if sorting can begin
			BubbleSort::Render(BubbleSort::GetPillarWidth()); //Renders
		}

		app.Display();
	}
}