void dumpString(char* array, int len)
{
	assert(array);
	int i = 0;

	printf("\n\t___________I'm dumping char array_%p_\n\n", array);
	for (i = 0; i < len; i++)
	{
		printf("'%c'  ", isprint(array[i]) ? array[i] : ' ');
	}
	printf("\n");
	for (i = 0; i < len; i++)
	{
		printf("0x%02x|", array[i]);
	}
	printf("\n\n\t___________Bye_____________________________\n");
}


