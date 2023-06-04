inherited FormImportBMP: TFormImportBMP
  Caption = 'Bitmap Import'
  ClientHeight = 205
  ClientWidth = 517
  ExplicitWidth = 523
  ExplicitHeight = 230
  PixelsPerInch = 96
  TextHeight = 16
  inherited GroupBox1: TGroupBox
    Left = 218
    Width = 299
    Height = 205
    Align = alRight
    Caption = 'Pass 2'
    ExplicitLeft = 218
    ExplicitWidth = 299
    ExplicitHeight = 205
    inherited GroupBox4: TGroupBox
      Top = 136
      ExplicitTop = 136
    end
    inherited GroupBox3: TGroupBox
      Top = 15
      Height = 51
      Caption = 'Colour index order'
      ExplicitTop = 15
      ExplicitHeight = 51
    end
    inherited GroupBox2: TGroupBox
      Top = 70
      ExplicitTop = 70
    end
    inherited ButtonSwap: TButton
      Top = 164
      Height = 32
      ExplicitTop = 164
      ExplicitHeight = 32
    end
  end
  object GroupBox6: TGroupBox
    Left = 0
    Top = 0
    Width = 220
    Height = 205
    Align = alLeft
    Caption = 'Pass 1'
    TabOrder = 1
    object GroupBox8: TGroupBox
      Left = 3
      Top = 14
      Width = 212
      Height = 132
      Caption = 'Options'
      TabOrder = 0
      object Label1: TLabel
        Left = 128
        Top = 19
        Width = 37
        Height = 13
        Caption = '# tiles: '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object BtnWdtInc: TSpeedButton
        Left = 119
        Top = 100
        Width = 33
        Height = 14
        Caption = '+16'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -9
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        OnClick = BtnWdtIncClick
      end
      object SpeedButton1: TSpeedButton
        Left = 119
        Top = 115
        Width = 33
        Height = 14
        Caption = '-16'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -9
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        OnClick = SpeedButton1Click
      end
      object CheckBestOffsets: TCheckBox
        Left = 14
        Top = 17
        Width = 97
        Height = 17
        Caption = 'best offsets'
        TabOrder = 0
        OnClick = CheckBestOffsetsClick
      end
      object CheckLossy: TCheckBox
        Left = 14
        Top = 80
        Width = 97
        Height = 17
        Caption = 'lossy (slow!)'
        TabOrder = 1
        OnClick = CheckBestOffsetsClick
      end
      object CheckDensityThres: TCheckBox
        Left = 14
        Top = 59
        Width = 149
        Height = 17
        Caption = 'pixel density threshold'
        TabOrder = 2
        OnClick = CheckBestOffsetsClick
      end
      object CheckNoAttr: TCheckBox
        Left = 14
        Top = 38
        Width = 87
        Height = 17
        Caption = 'skip attr'
        TabOrder = 3
        OnClick = CheckBestOffsetsClick
      end
      object EditPxThres: TEdit
        Left = 162
        Top = 52
        Width = 27
        Height = 24
        TabOrder = 4
        Text = '8'
        OnChange = CheckBestOffsetsClick
      end
      object UpDown1: TUpDown
        Left = 189
        Top = 52
        Width = 15
        Height = 24
        Associate = EditPxThres
        Max = 64
        Position = 8
        TabOrder = 5
      end
      object CheckMaxTiles: TCheckBox
        Left = 14
        Top = 101
        Width = 104
        Height = 17
        Caption = 'maximum tiles'
        TabOrder = 6
        OnClick = CheckBestOffsetsClick
      end
      object EditMaxTiles: TEdit
        Left = 158
        Top = 100
        Width = 31
        Height = 24
        TabOrder = 7
        Text = '64'
        OnExit = CheckBestOffsetsClick
      end
      object UpDown2: TUpDown
        Left = 189
        Top = 100
        Width = 15
        Height = 24
        Associate = EditMaxTiles
        Min = 1
        Max = 512
        Position = 64
        TabOrder = 8
      end
      object CheckNoPal: TCheckBox
        Left = 87
        Top = 38
        Width = 69
        Height = 17
        Caption = 'skip pal'
        TabOrder = 9
        OnClick = CheckBestOffsetsClick
      end
      object Button9: TButton
        Left = 119
        Top = 79
        Width = 33
        Height = 17
        Caption = '...'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -10
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
        Visible = False
        OnClick = Button9Click
      end
    end
    object GroupBox7: TGroupBox
      Left = 3
      Top = 146
      Width = 212
      Height = 52
      Caption = 'Method'
      TabOrder = 1
      object RadioAsMap: TRadioButton
        Left = 11
        Top = 15
        Width = 178
        Height = 17
        Caption = 'normal (import all) '
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = CheckBestOffsetsClick
      end
      object RadioMatched: TRadioButton
        Left = 11
        Top = 31
        Width = 178
        Height = 17
        Caption = 'matched to existing tileset'
        TabOrder = 1
        OnClick = CheckBestOffsetsClick
      end
    end
  end
end
