#ifndef PLAY_AKINATOR
#define PLAY_AKINATOR

enum
{
	MAX_NAME_LEN = 128
};

void Akinator::playAkinator(Node* node)
{
	FUNC_HI
	char answer[4] = {};
	char newName[MAX_NAME_LEN]  = {};
	char feature[MAX_NAME_LEN]  = {};
	Node* tempParent = NULL;
	if(node->leftC_ && node->rghtC_)
	{
		printf("\n\t%s\n", node->buf_);
		scanf(" %3s", answer); 	
		scanf("%*[^\n]");
		if(strcmp(answer, "yes") == 0) 
		{
			playAkinator(node->rghtC_);
		}
		else
			if(strcmp(answer, "no") == 0)
			{
				playAkinator(node->leftC_);
			}
			else
				{
					printf("\n\tPlease, answer only yes or no\n");
					playAkinator(node);
				}
	}
	else
	{
		printf("\n\tI guess it is %s, am I right?\n", node->buf_);
		scanf(" %3s", answer);
		if(strcmp(answer, "yes") == 0)
		{
			printf("\n\tWe have reached the answer!\n");
#ifdef DEBUG
				loadInFile(fileName);
#else
				loadInFile();
#endif
			return;
		}
		else			
			if(strcmp(answer, "no") == 0)
			{
				printf("\n\tSo, who is he?\n");
				assert(MAX_NAME_LEN == 128);
				scanf(" %127[^\n]", newName);
				printf("\n\tAnd what is the difference?\n");
				scanf(" %127[^\n]", feature);
				strcat(feature, "?");
				tempParent = node->parent_;
				Node* newFeature   = new Node(feature, tempParent);
				if(tempParent->rghtC_ == node)	
					tempParent->rghtC_  = newFeature;
				if(tempParent->leftC_ == node)
					tempParent->leftC_	= newFeature;

				newFeature->parent_ = tempParent;

				newFeature->rghtC_ = new Node(newName, newFeature);
				newFeature->rghtC_->parent_ = newFeature;
				newFeature->leftC_ = node;
				node->parent_ = newFeature;
				printf("\n\tThe new feature has been added\n");
#ifdef DEBUG
				loadInFile(fileName);
#else
				loadInFile();
#endif
			}
			else
				{
					printf("\n\tPlease, answer only yes or no\n");
					playAkinator(node);
				}

	}
	FUNC_BYE
}
#endif //PLAY_AKINATOR
