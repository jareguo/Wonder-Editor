let unsafeGetScene = () =>
  SceneEditorService.unsafeGetScene |> StateLogicService.getEditorState;

let setCameraTobeCurrentSceneTreeNode = () =>
  unsafeGetScene()
  |> GameObjectTool.getChildren
  |> Js.Array.filter(gameObject =>
       CameraEngineService.isCamera(gameObject)
       |> StateLogicService.getEngineStateToGetData
     )
  |> ArrayService.getFirst
  |> GameObjectTool.setCurrentSceneTreeNode;

let getBoxByIndex = (index, engineState) =>
  engineState
  |> GameObjectUtils.getChildren(unsafeGetScene())
  |> Js.Array.filter(gameObject =>
       ! CameraEngineService.isCamera(gameObject, engineState)
     )
  |> ArrayService.getNth(index);

let setFirstBoxTobeCurrentSceneTreeNode = () =>
  getBoxByIndex(0, StateLogicService.getRunEngineState())
  |> GameObjectTool.setCurrentSceneTreeNode;

/* let initStateAndGlWithJobAndGl =
       (
         ~sandbox,
         ~noWorkerJobRecord,
         ~buffer=SettingToolEngine.buildBufferConfigStr(),
         ~gl,
         (),
       ) => {
     TestTool.initEditorAndEngineStateAndInitSceneWithJob(
       ~sandbox,
       ~buffer,
       ~noWorkerJobRecord,
       (),
     );
     TestTool.openContractCheck();
     TestToolEngine.setFakeGlWithGl(gl);
     TestToolEngine.openContractCheck();
     AllMaterialToolEngine.prepareForInit();
     TestTool.rebuildAssetState() |> ignore;
   }; */

let initStateAndGlWithJob =
    (
      ~sandbox,
      ~noWorkerJobRecord,
      ~buffer=SettingToolEngine.buildBufferConfigStr(),
      (),
    ) => {
  /* initStateAndGlWithJobAndGl(
       ~sandbox,
       ~noWorkerJobRecord,
       ~buffer,
       ~gl=FakeGlToolEngine.buildFakeGl(~sandbox, ()),
       (),
     ); */

  TestTool.initEditorAndEngineStateAndInitSceneWithJob(
    ~sandbox,
    ~buffer,
    ~noWorkerJobRecord,
    (),
  );
  TestTool.openContractCheck();
  /* TestToolEngine.setFakeGlWithGl(gl); */
  TestToolEngine.openContractCheck();
  AllMaterialToolEngine.prepareForInit();
  TestTool.rebuildAssetState() |> ignore;
};

let initStateAndGl =
    (~sandbox, ~buffer=SettingToolEngine.buildBufferConfigStr(), ()) =>
  initStateAndGlWithJob(
    ~sandbox,
    ~noWorkerJobRecord=
      NoWorkerJobConfigToolEngine.buildNoWorkerEmptyJobConfig(),
    ~buffer,
    (),
  );

let createDefaultScene = (sandbox, initFunc) => {
  let scene = unsafeGetScene();
  let editorState = StateEditorService.getState();
  let editEngineState = StateLogicService.getEditEngineState();
  let (editEngineState, box) =
    editEngineState
    |> DefaultSceneUtils.prepareSpecificGameObjectsForEditEngineState(scene);
  let (editorState, editEngineState) =
    editEngineState |> DefaultSceneUtils.computeDiffValue(editorState);
  let (editEngineState, camera) =
    editEngineState |> DefaultSceneUtils.createDefaultScene(scene);

  editorState |> StateEditorService.setState |> ignore;
  editEngineState
  |> GameObjectUtils.setParentKeepOrder(camera, box)
  |> FakeGlToolEngine.setFakeGl(FakeGlToolEngine.buildFakeGl(~sandbox, ()))
  |> StateLogicService.setEditEngineState;

  let (runEngineState, _) =
    StateLogicService.getRunEngineState()
    |> DefaultSceneUtils.createDefaultScene(scene);

  runEngineState
  |> FakeGlToolEngine.setFakeGl(FakeGlToolEngine.buildFakeGl(~sandbox, ()))
  |> StateLogicService.setRunEngineState;
  initFunc();
};

let _isBox = (gameObject, engineState) =>
  GameObjectComponentEngineService.hasBoxGeometryComponent(
    gameObject,
    engineState,
  );

let getBoxInDefaultScene = engineState =>
  GameObjectUtils.getChildren(unsafeGetScene(), engineState)
  |> Js.Array.filter(gameObject => _isBox(gameObject, engineState))
  |> WonderCommonlib.ArrayService.unsafePop;