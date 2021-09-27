TEST(AddElementToListTest, nullptrWord_errcode_2) {

  LST* list = NULL;
  int err;

  err = AddElementToList(&list, 1, NULL);

  EXPECT_EQ(err, 2);
}

TEST(AddElementToListTest, addingNewElement)
{
  LST* list = NULL;
  AddElementToList(&list, 10, "word");
  EXPECT_EQ(list->keyDigit,10);
  EXPECT_TRUE(!strcmp(list->word, "word"));
}

TEST(AddElementToListTest, sort)
{
  LST* list = NULL;

  AddElementToList(&list, 4, "4");
  AddElementToList(&list, 2, "2");
  AddElementToList(&list, 3, "3");

  EXPECT_TRUE(list->keyDigit < list->ptrToNext->keyDigit);
  EXPECT_TRUE(list->ptrToNext->keyDigit < list->ptrToNext->ptrToNext->keyDigit);
}


TEST(FindElementInListTest, notExistingElement_expect_0)
{
  LST list, list2;
  int output;
  list.keyDigit = 1;
  list.word[0] = 0;
  list.ptrToNext = &list2;
  list2.keyDigit = 2;
  list2.word[0] = 0;
  list2.ptrToNext = &list2;
  output = FindElementInList(&list, 3);
  EXPECT_EQ(output,0);
}

TEST(FindElementInListTest, existingElement_expect_1)
{
  LST list, list2;
  int output;
  list.keyDigit = 1;
  list.word[0] = 0;
  list.ptrToNext = &list2;
  list2.keyDigit = 2;
  list2.word[0] = 0;
  list2.ptrToNext = &list2;
  output = FindElementInList(&list, 2);
  EXPECT_EQ(output, 1);
  output = FindElementInList(&list, 1);
  EXPECT_EQ(output, 1);
}
